/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * An implementation of the InputStream to read serial data out from the flash
 * IC. The data is read out in batches using DMA. We try not to block while the
 * DMA is working in an attempt to get the most out of our cycles.
 */

class FlashInputStream : public Flash,
                         public InputStream {

  protected:
    enum {
      BUFFER_SIZE = 768      // how many bytes read out from the flash per batch
    };

    uint8_t _buffer[BUFFER_SIZE];   // the internal buffer
    uint32_t _bytesRemaining;       // bytes remaining to read out
    uint32_t _bufferPos;            // current position in the buffer
    bool _dmaActive;                // true when a read has not been confirmed done

    /*
     * Declare the SPI1 DMA channel that we'll use. It operates in 8-bit mode
     */

    typedef Spi1RxDmaChannel<SpiDmaReaderFeature<Spi1PeripheralTraits> > MyRxDma;
    typedef Spi1TxDmaChannel<SpiDmaWriterFeature<Spi1PeripheralTraits> > MyTxDma;

    MyRxDma _rxdma;
    MyTxDma _txdma;

    SpiNssManager _nss;       // this class holds NSS low while in scope

  protected:
    void beginRefreshBuffer();
    void waitPending();

  public:
    FlashInputStream(uint32_t sourceAddress,uint32_t totalSize);
    virtual ~FlashInputStream();

    // overrides from InputStream

    virtual int16_t read() override;
    virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
    virtual bool skip(uint32_t howMuch) override;
    virtual bool available() override;
    virtual bool reset() override;

    // overrides from StreamBase

    virtual bool close() override;
};


/*
 * Wait for pending DMA transfer to complete
 */

inline void FlashInputStream::waitPending() {

  if(_dmaActive) {
    while(!_rxdma.isComplete());
    _dmaActive=false;
  }
}


/*
 * Constructor
 */

inline FlashInputStream::FlashInputStream(uint32_t sourceAddress,uint32_t totalSize) :
  _bytesRemaining(totalSize),
  _bufferPos(BUFFER_SIZE),
  _dmaActive(false),
  _nss(*_spi) {

  uint8_t bytes[4];

  // first 32-bits are the read command and the offset

  bytes[0]=READ_DATA;
  bytes[1]=sourceAddress >> 16;
  bytes[2]=sourceAddress >> 8;
  bytes[3]=sourceAddress;

  // write out as four 8-bit transfers

  _spi->send(bytes,4);

  // speculatively start the first read

  beginRefreshBuffer();
}


/*
 * Destructor
 */

inline FlashInputStream::~FlashInputStream() {

  // don't want to inadvertently release the buffer if DMA
  // is still using it. Not likely in our use case but let's be safe.

  waitPending();
}


/*
 * Read single
 */

inline int16_t FlashInputStream::read() {

  uint8_t byte;

  // check for end

  if(_bytesRemaining==0)
    return E_END_OF_STREAM;

  // wait for anything pending

  waitPending();

  // get the next byte

  _bytesRemaining--;
  byte=_buffer[_bufferPos++];

  // read-ahead if required

  beginRefreshBuffer();

  // return this byte

  return byte;
}


/*
 * Read multiple
 */

inline bool FlashInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

  uint8_t *ptr;
  uint32_t count,inbuffer;

  // notionally continue until none left

  actuallyRead=0;
  ptr=reinterpret_cast<uint8_t *>(buffer);

  while(size) {

    // check for nothing left (not an error)

    if(_bytesRemaining==0)
      return true;

    // wait for any pending

    waitPending();

    // this time around we'll read up to size bytes

    inbuffer=BUFFER_SIZE-_bufferPos;
    count=size<inbuffer ? size : inbuffer;

    // read it out

    memcpy(ptr,_buffer+_bufferPos,count);

    // update positions and sizes

    ptr+=count;
    actuallyRead+=count;
    size-=count;

    _bufferPos+=count;
    _bytesRemaining-=count;

    // ensure data is available

    beginRefreshBuffer();
  }

  return true;
}


/*
 * Skip forward: used by the LZG decoder to skip past the header
 */

inline bool FlashInputStream::skip(uint32_t howMuch) {

  // not efficient, but doesn't really need to be

  while(howMuch--)
    if(read()<0)
      return false;

  return true;
}


/*
 * Check if data is available
 */

inline bool FlashInputStream::available() {
  return _bytesRemaining!=0;
}


/*
 * Reset to start: not supported
 */

inline bool FlashInputStream::reset() {
  return false;
}


/*
 * Close: unsupported
 */

inline bool FlashInputStream::close() {
  return false;
}


/*
 * Start a refresh the internal buffer if data is available and the internal buffer
 * has run out of bytes
 */

inline void FlashInputStream::beginRefreshBuffer() {

  uint8_t zero;
  uint32_t transferSize;

  // do nothing if data has yet to be consumed

  if(_bufferPos<BUFFER_SIZE && _bytesRemaining>0)
    return;

  // do nothing if no data remaining

  if(_bytesRemaining==0)
    return;

  // transfer up to the buffer size

  if(_bytesRemaining<BUFFER_SIZE)
    transferSize=_bytesRemaining;
  else
    transferSize=BUFFER_SIZE;

  // start the full transfer (tx is required with SPI or the clock won't tick)

  _rxdma.beginRead(_buffer,transferSize);
  _txdma.beginWrite(&zero,transferSize);

  // flag that DMA is active

  _dmaActive=true;

  // reset the consumer pointer. the read() methods will block until the DMA transfer
  // has completed.

  _bufferPos=0;
}
