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
