/*
**
*/

#include <proto/bgui.h>
#include <libraries/bgui.h>
#include <libraries/bgui_macros.h>

struct Library *BGUIBase;

#define ID_CMD1_STRING  1
#define ID_CMD1_CYCLE   2
#define ID_CMD2_STRING  3
#define ID_CMD2_CYCLE   4
#define ID_CMD3_STRING  5
#define ID_CMD3_CYCLE   6
#define ID_CMD4_STRING  7
#define ID_CMD4_CYCLE   8
#define ID_CMD5_STRING  9
#define ID_CMD5_CYCLE   10
#define ID_CMD6_STRING  11
#define ID_CMD6_CYCLE   12
#define ID_CMD7_STRING  13
#define ID_CMD7_CYCLE   14
#define ID_CMD8_STRING  15
#define ID_CMD8_CYCLE   16
#define ID_CMD9_STRING  17
#define ID_CMD9_CYCLE   18
#define ID_CMD10_STRING 19
#define ID_CMD10_CYCLE  20
#define ID_CMD11_STRING 21
#define ID_CMD11_CYCLE  22
#define ID_CMD12_STRING 23
#define ID_CMD12_CYCLE  24
#define ID_CMD13_STRING 25
#define ID_CMD13_CYCLE  26
#define ID_CMD14_STRING 27
#define ID_CMD14_CYCLE  28
#define ID_CMD15_STRING 29
#define ID_CMD15_CYCLE  30
#define ID_CMD16_STRING 31
#define ID_CMD16_CYCLE  32
#define ID_SAVE_BUTTON  35
#define ID_QUIT_BUTTON  36

static UBYTE *cycle_labels[] =
{
   "ARexx",
   "AmigaDOS",
   "CNetC",
   NULL
};

void main(void)
{
   Object *WO_Window;
   struct Window *window;

   Object *BO_Save,
          *BO_Quit,
          *SO_Cmd1,
          *SO_Cmd2,
          *SO_Cmd3,
          *SO_Cmd4,
          *SO_Cmd5,
          *SO_Cmd6,
          *SO_Cmd7,
          *SO_Cmd8,
          *SO_Cmd9,
          *SO_Cmd10,
          *SO_Cmd11,
          *SO_Cmd12,
          *SO_Cmd13,
          *SO_Cmd14,
          *SO_Cmd15,
          *SO_Cmd16,
          *SO_Cmd17,
          *CO_Cmd1,
          *CO_Cmd2,
          *CO_Cmd3,
          *CO_Cmd4,
          *CO_Cmd5,
          *CO_Cmd6,
          *CO_Cmd7,
          *CO_Cmd8,
          *CO_Cmd9,
          *CO_Cmd10,
          *CO_Cmd11,
          *CO_Cmd12,
          *CO_Cmd13,
          *CO_Cmd14,
          *CO_Cmd15,
          *CO_Cmd16,
          *CO_Cmd17;
         

   if(BGUIBase = OpenLibrary(BGUINAME, BGUIVERSION))
   {
      WO_Window = WindowObject,
         WINDOW_Title, "CINS Command Configuration",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset,
               StartMember,
                  ViewObject, VIEW_MinWidth, 400, VIEW_ScaleMinHeight, 25, VIEW_Object,
                     VGroupObject, NormalOffset,
                        VarSpace(DEFAULT_WEIGHT),
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd1 = TabString("Command 1:", "", 256, ID_CMD1_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd1 = PopCycle("", cycle_labels, NULL, ID_CMD1_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd2 = TabString("Command 2:", "", 256, ID_CMD2_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd2 = PopCycle("", cycle_labels, NULL, ID_CMD2_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd3 = TabString("Command 3:", "", 256, ID_CMD3_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd3 = PopCycle("", cycle_labels, NULL, ID_CMD3_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd4 = TabString("Command 4:", "", 256, ID_CMD4_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd4 = PopCycle("", cycle_labels, NULL, ID_CMD4_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd5 = TabString("Command 5:", "", 256, ID_CMD5_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd5 = PopCycle("", cycle_labels, NULL, ID_CMD5_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd6 = TabString("Command 6:", "", 256, ID_CMD6_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd6 = PopCycle("", cycle_labels, NULL, ID_CMD6_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd7 = TabString("Command 7:", "", 256, ID_CMD7_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd7 = PopCycle("", cycle_labels, NULL, ID_CMD7_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd8 = TabString("Command 8:", "", 256, ID_CMD8_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd8 = PopCycle("", cycle_labels, NULL, ID_CMD8_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd9 = TabString("Command 9:", "", 256, ID_CMD9_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd9 = PopCycle("", cycle_labels, NULL, ID_CMD9_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd10 = TabString("Command 10:", "", 256, ID_CMD10_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd10 = PopCycle("", cycle_labels, NULL, ID_CMD10_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd11 = TabString("Command 11:", "", 256, ID_CMD11_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd11 = PopCycle("", cycle_labels, NULL, ID_CMD11_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd12 = TabString("Command 12:", "", 256, ID_CMD12_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd12 = PopCycle("", cycle_labels, NULL, ID_CMD12_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd13 = TabString("Command 13:", "", 256, ID_CMD13_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd13 = PopCycle("", cycle_labels, NULL, ID_CMD13_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd14 = TabString("Command 14:", "", 256, ID_CMD14_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd14 = PopCycle("", cycle_labels, NULL, ID_CMD14_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd15 = TabString("Command 15:", "", 256, ID_CMD15_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd15 = PopCycle("", cycle_labels, NULL, ID_CMD15_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        StartMember,
                           HGroupObject,
                              StartMember,
                                 SO_Cmd16 = TabString("Command 16:", "", 256, ID_CMD16_STRING),
                              EndMember,
                              StartMember,
                                 CO_Cmd16 = PopCycle("", cycle_labels, NULL, ID_CMD16_CYCLE),
                                 FixMinWidth,
                              EndMember,
                           EndObject,
                        EndMember,
                        VarSpace(DEFAULT_WEIGHT),
                     EndMember,
                  EndObject, EndObject,
               EndMember,
               StartMember,
                  HGroupObject,
                     StartMember,
                        BO_Save = PrefButton("_Save", ID_SAVE_BUTTON),
                     EndMember,
                     StartMember,
                        BO_Quit = PrefButton("_Quit", ID_QUIT_BUTTON),
                     EndMember,
                  EndObject, FixMinHeight,
               EndMember,
            EndObject,
      EndObject;

      if(WO_Window)
      {
         ULONG signal = 0,
               rc;

         BOOL done = FALSE;

         if(window = WindowOpen(WO_Window))
         {
            GetAttr(WINDOW_SigMask, WO_Window, &signal);

            do
            {
               while((rc = HandleEvent(WO_Window)) != WMHI_NOMORE)
               {
                  switch(rc)
                  {
                     case ID_QUIT_BUTTON:
                        done = TRUE;
                        break;
                  }
               }
            } while (done != TRUE);
         }
         DisposeObject(WO_Window);
      }

      CloseLibrary(BGUIBase);
   }
}
