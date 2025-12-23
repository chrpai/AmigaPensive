Beispiel zur Behandlung von PMBS-Dimfeldern in C
(die Brettliste wird z.B. in DimFeldern verwaltet)

      #include <proto/exec.h>

      struct DimFeld
      {
        long DimAnzahl;
        void *DimData[0];  // erzeugt ANSI-Portablilit„tswarnung !!
      };

      struct DimFeld *GetDimFeld(long offset)
      {
        struct MsgPort *PServer;
        struct DimFeld *DimFeld;

        PServer=FindPort("rexx_PXServer");

        if(PServer)
        {
          DimFeld=(struct DimFeld *) (* ( (ULONG *) ( * ( (ULONG *)
                      ((UBYTE *) PServer + offset ))))) ;

          return DimFeld;
        }
        else
          return (struct DimFeld *)0L;
      }
