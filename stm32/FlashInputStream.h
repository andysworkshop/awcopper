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
      BUFFER_SIZE = 1024      // how many bytes read out from the flash per batch
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
