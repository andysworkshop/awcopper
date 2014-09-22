/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace awreflow {

  /*
   * Base flash class looks after the SPI and DMA communications
   */

  class Flash {

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

      bool eraseLastSector() const;
      bool writeLastPage(const uint8_t *page) const;
      bool readLastPage(uint8_t *page) const;
  };
}
