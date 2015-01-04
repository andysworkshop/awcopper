/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


/*
 * Base flash class looks after the SPI and DMA communications
 */

class Flash {

  public:

    /*
     * Sector sizes used in the erase command
     */

    enum SectorSize {
      SECTOR_4K,
      SECTOR_8K,
      SECTOR_64K
    };

    /*
     * Commands used here
     */

    enum {
      PAGE_PROGRAM              = 0x02,
      READ_DATA                 = 0x03,
      PARAMETER_SECTOR_ERASE_4K = 0x20,
      PARAMETER_SECTOR_ERASE_8K = 0x40,
      BULK_ERASE                = 0x60,
      SECTOR_ERASE              = 0xd8
    };

  protected:

    /*
     * Declare the SPI peripheral that we'll use both directly and indirectly via DMA.
     */

    typedef Spi1<SpiFifoNotifyQuarterFullFeature> MySpi;
    scoped_ptr<MySpi> _spi;

    /*
     * Inner class to manage the NSS pin using the
     * Resource Acquisition Is Initialisation technique
     */

    struct SpiNssManager {
      const MySpi& _spi;

      SpiNssManager(const MySpi& spi)
        : _spi(spi) {
        _spi.setNss(false);
      }
      ~SpiNssManager() {
        _spi.setNss(true);
      }
    };

  protected:
    bool waitForIdle() const;
    bool writeEnable() const;
    bool readStatusRegister(uint8_t& sr) const;

  public:
    Flash();

    bool bulkErase() const;
    bool eraseSector(uint32_t sectorAddress,SectorSize sectorSize) const;
    bool writePage(uint32_t pageAddress,const uint8_t *page) const;
};


/*
 * Constructor
 */

inline Flash::Flash() {

  // create the peripheral. this will initialise it

  MySpi::Parameters params;

  params.spi_mode=SPI_Mode_Master;
  params.spi_baudRatePrescaler=SPI_BaudRatePrescaler_2;     // fastest mode = PCLK/2 = 48/2 = 24MHz
  params.spi_dataSize=SPI_DataSize_8b;                      // 8 bit transfers
  params.spi_cpol=SPI_CPOL_Low;                             // SPI mode 0
  params.spi_cpha=SPI_CPHA_1Edge;

  // create the SPI peripheral

  _spi.reset(new MySpi(params));
}


/*
 * Erase a sector from the device by setting all the bits to '1'. A sector must be erased
 * before the pages within it can be programmed.
 */

inline bool Flash::eraseSector(uint32_t sectorAddress,SectorSize sectorSize) const {

  uint8_t command[4];
  bool retval;

  // create the command

  command[0]=sectorSize==SECTOR_4K ? PARAMETER_SECTOR_ERASE_4K :
             sectorSize==SECTOR_8K ? PARAMETER_SECTOR_ERASE_8K : SECTOR_ERASE;

  // 24 bit address

  command[1]=sectorAddress >> 16;
  command[2]=sectorAddress >> 8 ;
  command[3]=sectorAddress;

  // sector erase requires write-enable

  if(!writeEnable())
    return false;

  // send the command and address

  _spi->setNss(false);
  retval=_spi->send(command,sizeof(command));
  _spi->setNss(true);

  // wait for it to finish

  return retval ? waitForIdle() : false;
}


/*
 * Bulk erase the entire flash device
 */

inline bool Flash::bulkErase() const {

  uint8_t command;
  bool retval;

  // sector erase requires write-enable

  if(!writeEnable())
    return false;

  // send the simple command

  command=BULK_ERASE;

  _spi->setNss(false);
  retval=_spi->send(&command,1);
  _spi->setNss(true);

  // wait for it to finish

  return retval ? waitForIdle() : false;
}


/*
 * Write a page (256 bytes)
 */

inline bool Flash::writePage(uint32_t address,const uint8_t *page) const {

  uint8_t command[4];

  // create the command (0x02 + address)

  command[0]=PAGE_PROGRAM;
  command[1]=address >> 16;
  command[2]=address >> 8;
  command[3]=address;

  // page program requires write-enable

  if(!writeEnable())
    return false;

  // send the 4 byte command. nss must go high after the page program or the
  // write will not happen.

  {
    SpiNssManager mgr(*_spi);

    if(!_spi->send(command,sizeof(command)))
      return false;

    // send the 256 bytes

    if(!_spi->send(page,256))
      return false;
  }

  return waitForIdle();
}


/*
 * Read the SPI device status register. We need to do this so we can
 * see the IDLE bit
 */

inline bool Flash::readStatusRegister(uint8_t& sr) const {

  uint8_t command;

  command=0x5;

  // send the 1 byte command

  SpiNssManager nss(*_spi);

  if(!_spi->send(&command,1))
    return false;

  // receive the single byte

  return _spi->receive(&sr,1);
}


/*
 * Wait for the flash device to become idle
 */

inline bool Flash::waitForIdle() const {

  uint8_t sr;

  do {

    if(!readStatusRegister(sr))
      return false;

  } while((sr & 1)!=0);     // bit zero is WIP (write in progress)

  return true;
}


/*
 * Send the write-enable command
 */

inline bool Flash::writeEnable() const {

  uint8_t command;

  command=0x6;

  // send the 1 byte command

  SpiNssManager nss(*_spi);
  return _spi->send(&command,1);
}
