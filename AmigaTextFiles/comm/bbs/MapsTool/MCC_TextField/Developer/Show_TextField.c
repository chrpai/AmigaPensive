#include "demo.h"
#include "MCC_TextField/developer/include/mui/textfield_mcc.h"
#define ID_PRINTCONTENTS 1


SAVEDS ASM VOID FileEditor(REG(a2) Object *Editor1,REG(a1) APTR *data);
void kprintf(UBYTE *fmt,...);     


static const struct Hook FileHook = { { 0,0 }, (VOID *)FileEditor, NULL, NULL };

SAVEDS ASM VOID FileEditor(REG(a2) Object *Editor1,REG(a1) APTR *data)
{
        LONG file = NULL;
        
        if (data[0]) get(data[0],MUIA_String_Contents,&file);
        if (Editor1) DoMethod(Editor1,data[1],file);
}  
 


int main(int argc, char *argv[]) {
  
  APTR app,WI_Main,Editor1,pop1,bt1,bt2,bt3,bt4,bt5,cm1,Scroll,textpen1,backgroundpen1,cursorpen1;
  BOOL running = TRUE;
  ULONG signals;
  char *editcontents=NULL;
  struct MUI_PenSpec *textpen;
  struct MUI_PenSpec *backgroundpen;
  struct MUI_PenSpec *cursorpen;
     
  init();


  app = ApplicationObject,
     MUIA_Application_Title,          "MUIEdit",
     MUIA_Application_Version,        "$VER 0.01 (18.11.95)",
     MUIA_Application_Copyright,      "© 1995, by Karl Bellve",
     MUIA_Application_Author,         "Karl Bellve",
     MUIA_Application_Description,    "Editor",
     MUIA_Application_Base,           "MUIEdit",
     
     SubWindow, WI_Main = WindowObject, 
          MUIA_Window_Title, "Test Editor",
          MUIA_Window_ID, MAKE_ID('C','O','N','T'),
          
          WindowContents, HGroup,
            
            Child, HGroup,
              Child, Scroll = ScrollgroupObject, MUIA_Scrollgroup_Contents, 
                 Editor1 = Editor, MUIA_TextField_Contents,"Hello\nMy name is Karl\n This is a textfield to be used with MUI3.3 and greater. If you have trouble with it or you have a bug report, mail me at kbellve@umabnet.ab.umd.edu and I will get back to you.\n\nThanks\nKarl\0",
                 End,
               End,
               Child, VGroup,
                 Child, VSpace(0),
                 Child, bt1 = SimpleButton("Get Contents"),
                 Child, bt2 = SimpleButton("Clear Contents"),
                 Child, cm1 = CheckMark(TRUE),
                 Child, ColGroup(2),
                    Child, KeyLabel2("File:",'f'),
					         Child, pop1 = PopaslObject,
						      MUIA_Popstring_String, KeyString(0,80,'i'),
						      MUIA_Popstring_Button, PopButton(MUII_PopUp),
						      MUIA_Popasl_Type , ASL_FileRequest,
						      ASLFO_TitleText  , "Please select a file...",
						   End,
                    Child, bt3 = SimpleButton("Save File"),
                    Child, bt4 = SimpleButton("Load File"),
                    Child, bt5 = SimpleButton("External Editor"),
                 End,
                 Child, HGroup, MUIA_Weight, 0,
                    Child, textpen1 = PoppenObject,
						      MUIA_Window_Title, "Text Color",
						   
					      End,
                    Child, backgroundpen1 = PoppenObject,
						      MUIA_Window_Title, "Background Color",
						   
					      End,
                    Child, cursorpen1 = PoppenObject,
						      MUIA_Window_Title, "Cursor Color",
						   
					      End,
                  VSpace(0),
                 End,
              End,
            End,
         End,
     End,
  End;
 
  if (!app) fail(NULL,"Failed to Create Application!\n");

  DoMethod(WI_Main,MUIM_Notify,MUIA_Window_CloseRequest,
    TRUE,app,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

  DoMethod(bt1,MUIM_Notify,MUIA_Pressed,
    FALSE,app,2,MUIM_Application_ReturnID,ID_PRINTCONTENTS);
  
  DoMethod(bt2,MUIM_Notify,MUIA_Pressed,
    FALSE,Editor1,2,MUIM_TextField_Clear,MUIV_TextField_Clear_All);
  
  DoMethod(bt5,MUIM_Notify,MUIA_Pressed,
    FALSE,Editor1,2,MUIM_TextField_ExternalEditor,TRUE);



  
  DoMethod(bt3,MUIM_Notify,MUIA_Pressed,FALSE,Editor1,4,MUIM_CallHook,&FileHook,pop1,MUIM_TextField_Save);

  DoMethod(bt4,MUIM_Notify,MUIA_Pressed,FALSE,Editor1,4,MUIM_CallHook,&FileHook,pop1,MUIM_TextField_Load);

  DoMethod(cm1,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,Scroll,3,MUIM_Set,MUIA_ShowMe,MUIV_TriggerValue);

  DoMethod(textpen1,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,
		Editor1,3,MUIM_Set,MUIA_TextField_TextPen,MUIV_TriggerValue);

  DoMethod(backgroundpen1,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,
		Editor1,3,MUIM_Set,MUIA_TextField_BackgroundPen,MUIV_TriggerValue);

  DoMethod(cursorpen1,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,
		Editor1,3,MUIM_Set,MUIA_TextField_CursorPen,MUIV_TriggerValue);
  
  
  DoMethod(textpen1,MUIM_Pendisplay_SetMUIPen,DEFAULT_TEXTPEN);
  
  DoMethod(backgroundpen1,MUIM_Pendisplay_SetMUIPen,DEFAULT_BACKGROUNDPEN);
    
  DoMethod(cursorpen1,MUIM_Pendisplay_SetMUIPen,DEFAULT_CURSORPEN);  
  
  
  get(textpen1,MUIA_Pendisplay_Spec,&textpen);
	set(Editor1,MUIA_TextField_TextPen,textpen);
  
  
  get(backgroundpen1,MUIA_Pendisplay_Spec,&backgroundpen);
	set(Editor1,MUIA_TextField_BackgroundPen,backgroundpen);
  
  
  get(cursorpen1,MUIA_Pendisplay_Spec,&cursorpen);
	set(Editor1,MUIA_TextField_CursorPen,cursorpen);

  set(WI_Main,MUIA_Window_Open,TRUE);
	set(WI_Main, MUIA_Window_ActiveObject, Editor1 );
  
  DoMethod(Editor1,MUIM_TextField_Move,MUIV_TextField_Move_Top);

  while (running) {
     switch(DoMethod(app,MUIM_Application_Input,&signals)){
    case MUIV_Application_ReturnID_Quit: 
         running = FALSE;
         break;
       case ID_PRINTCONTENTS: 
            get(Editor1,MUIA_TextField_Contents,&editcontents);
            if (editcontents) printf("%s",editcontents);
            break;

    }
    if (running && signals) Wait(signals);            

  }
  
  set(WI_Main,MUIA_Window_Open,FALSE);

  fail(app,NULL);    
}
