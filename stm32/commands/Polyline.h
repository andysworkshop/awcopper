/*
 * Andy's Workshop Arduino Graphics Coprocessor.
 * Copyright (c) 2014 Andy Brown. All rights reserved.
 * Please see website (http://www.andybrown.me.uk) for full details.
 */

#pragma once


namespace cmd {

  /*
   * Draw multiple lines, end to end
   *
   * Parameters:
   *   0 : number of line segments
   *   [ 1+n,2+n   : X
   *     3+n,4+n ] : Y
   *
   *   Array of points follows the point count. Must be at least 2 points
   */

  class Polyline {

    protected:
      static void readPoint(Point& p,ManagedCircularBuffer& commandBuffer);

    public:
      static void execute(Panel& panel,ManagedCircularBuffer& commandBuffer);
  };


  /*
   * Execute the command
   */

  inline void Polyline::execute(Panel& panel,ManagedCircularBuffer& commandBuffer) {

    Panel::LcdPanel& gl(panel.getGraphicsLibrary());
    uint8_t points;
    Point p[2];

    // read the number of points

    if((points=commandBuffer.managedRead())<2)
      return;

    // read first point

    readPoint(p[1],commandBuffer);
    points--;

    do {

      // copy previous last point to first point and read new last point

      p[0]=p[1];
      readPoint(p[1],commandBuffer);

      // draw the line and loop while there's more to do

      gl.drawLine(p[0],p[1]);
    } while(--points);
  }


  /*
   * Read a single point (X,Y) from the buffer
   */

  inline void Polyline::readPoint(Point& p,ManagedCircularBuffer& commandBuffer) {
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&p.X),sizeof(p.X));
    commandBuffer.managedRead(reinterpret_cast<uint8_t *>(&p.Y),sizeof(p.Y));
  }
}
