/* ----------------------------------------------------------------------
   LIGGGHTS - LAMMPS Improved for General Granular and Granular Heat
   Transfer Simulations

   LIGGGHTS is part of the CFDEMproject
   www.liggghts.com | www.cfdem.com

   Christoph Kloss, christoph.kloss@cfdem.com
   Copyright 2009-2012 JKU Linz
   Copyright 2012-     DCS Computing GmbH, Linz

   LIGGGHTS is based on LAMMPS
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   This software is distributed under the GNU General Public License.

   See the README file in the top-level directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing authors:
   Christoph Kloss (JKU Linz, DCS Computing GmbH, Linz)
   Philippe Seil (JKU Linz)
------------------------------------------------------------------------- */

#ifndef LMP_CONTAINER_BASE_H
#define LMP_CONTAINER_BASE_H

namespace LAMMPS_NS
{
  // buffer operation types (for push and pop)

  enum{ OPERATION_COMM_EXCHANGE,
        OPERATION_COMM_BORDERS,
        OPERATION_COMM_FORWARD,
        OPERATION_COMM_REVERSE,
        OPERATION_RESTART,
        OPERATION_UNDEFINED};

  class ContainerBase
  {
      public:

          //NP need to make this virtual to have the correct destructor of derived class
          //NP called via delete in AssociativePointerArray class
          virtual ~ContainerBase();

          void setProperties(char *_id, char* _comm, char* _ref, char *_restart,int _scalePower = 1);
          bool propertiesSetCorrectly();

          void id(char *_id);
          bool matches_id(char *_id);

          virtual void addUninitialized(int n) = 0;
          virtual int size() = 0;
          virtual int nVec() = 0;
          virtual int lenVec() = 0;

          virtual void copy(int from,int to) = 0;
          virtual void del(int n) = 0;
          virtual void delForward(int n,bool scale,bool translate,bool rotate) = 0;
          virtual void delRestart(int n,bool scale,bool translate,bool rotate) = 0;
          virtual void clearReverse(bool scale,bool translate,bool rotate) = 0;

          virtual void scale(double factor) = 0;
          virtual void move(double *dx) = 0;
          virtual void moveElement(int i,double *dx) = 0;
          virtual void rotate(double *dQ) = 0;

          // buffer functions for parallelization

          virtual int bufSize(int operation = OPERATION_UNDEFINED,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int popFromBuffer(double *buf,int operation,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int pushToBuffer(double *buf,int operation,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;

          virtual int elemListBufSize(int n, int operation = OPERATION_UNDEFINED,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int pushElemListToBuffer(int n, int *list, double *buf, int operation,
                           bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int popElemListFromBuffer(int first, int n, double *buf, int operation,
                           bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int pushElemListToBufferReverse(int first, int n, double *buf, int operation,
                           bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int popElemListFromBufferReverse(int n, int *list, double *buf, int operation,
                           bool scale=false,bool translate=false, bool rotate=false) = 0;

          virtual int elemBufSize(int operation = OPERATION_UNDEFINED,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int pushElemToBuffer(int n, double *buf,int operation,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;
          virtual int popElemFromBuffer(double *buf,int operation,
                            bool scale=false,bool translate=false, bool rotate=false) = 0;

      protected:

          ContainerBase();
          ContainerBase(char *_id, char* _comm, char* _ref, char *_restart,int _scalePower);
          ContainerBase(ContainerBase const &orig);

          inline bool isScaleInvariant();
          inline bool isTranslationInvariant();
          inline bool isRotationInvariant();

          //NP decide on wheater at all an operation is performed here
          inline bool decidePackUnpackOperation(int operation,bool scale,bool translate, bool rotate);

          //NP decide if operation performs data communication
          inline bool decideCommOperation(int operation);

          //NP decide if unpack creates new element or overwrites existing data
          inline bool decideCreateNewElements(int operation);


          char *id_;
          int communicationType_;
          int refFrame_;
          int scalePower_;
          int restartType_;
  };

  // *************************************
  #include "container_base_I.h"
  // *************************************

} /* LAMPPS_NS */
#endif /* CONTAINERBASE_H_ */
