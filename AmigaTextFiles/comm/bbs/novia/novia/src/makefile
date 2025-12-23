NOVIA_MAIN=novia_main.o
OBTIONS=cppc -c -pc -w1000 
novia: novia_global.o\
	$(NOVIA_MAIN)\
	NewUserSequence.o\
	networks/novia_serial.o\
	novia_io_main.o\
	novia_visualeditor.o\
	novia_user.o\
	api/novia_tools_api.o\
	api/novia_dos_api.o\
	api/novia_io_api.o\
	api/novia_nrc_api.o\
	api/novia_tcpip_api.o\
	services/novia_nfs_server.o\
	services/novia_nrc_server.o\
	novia_rexx.o\
	novia_xpr.o\
	novia_gui_data.o\
	novia_gui.o\
	novia_reg_data.o\
	novia_gui_methods.o\
	novia_baseviewer.o\
	novia_baseview_data.o\
	novia_syslibrary.o\
	gui/novia_gui_converts.o\
	novia_nsl.o\
	novia_myget.o\
	novia_internal_commands.o
 cppc \
	novia_global.o \
	$(NOVIA_MAIN)\
	NewUserSequence.o\
	networks/novia_serial.o\
	novia_io_main.o\
	novia_visualeditor.o\
	novia_user.o\
	api/novia_tools_api.o\
	api/novia_dos_api.o\
	api/novia_io_api.o\
	api/novia_nrc_api.o\
	api/novia_tcpip_api.o\
	services/novia_nfs_server.o\
	services/novia_nrc_server.o\
	novia_rexx.o\
	novia_xpr.o\
	novia_gui_data.o\
	novia_gui.o\
	novia_reg_data.o\
	novia_gui_methods.o\
	novia_baseviewer.o\
	novia_baseview_data.o\
	novia_syslibrary.o\
	gui/novia_gui_converts.o\
	novia_nsl.o\
	novia_myget.o\
	novia_internal_commands.o\
	\
	-o novia -l amiga

novia_global.o: novia_global.c
	$(OBTIONS) novia_global.c

$(NOVIA_MAIN): novia_main.c
	$(OBTIONS) novia_main.c

NewUserSequence.o: NewUserSequence.c
	$(OPTIONS) NewUserSequence.c

networks/novia_serial.o: networks/novia_serial.c
	$(OPTIONS) networks/novia_serial.c

novia_io_main.o: novia_io_main.c
	$(OPTIONS) novia_io_main.c

novia_visualeditor.o: novia_VisualEditor.c
	$(OPTIONS) novia_VisualEditor.c

novia_user.o: novia_user.c
	$(OPTIONS) novia_user.c

api/novia_tools_api.o: api/novia_tools_api.c
	$(OPTIONS) api/novia_tools_api.c

api/novia_dos_api.o: api/novia_dos_api.c
	$(OPTIONS) api/novia_dos_api.c

api/novia_io_api.o: api/novia_io_api.c
	$(OPTIONS) api/novia_io_api.c

api/novia_tcpip_api.o: api/novia_tcpip_api.c
	$(OPTIONS) api/novia_tcpip_api.c

api/novia_nrc_api.o: api/novia_nrc_api.c
	$(OPTIONS) api/novia_nrc_api.c

services/novia_nfs_server.o: services/novia_nfs_server.c
	$(OPTIONS) services/novia_nfs_server.c

services/novia_nrc_server.o: services/novia_nrc_server.c
	$(OPTIONS) services/novia_nrc_server.c

novia_rexx.o: novia_rexx.c
	$(OPTIONS) novia_rexx.c

novia_xpr.o: novia_xpr.c
	$(OPTIONS) novia_xpr.c

novia_gui_data.o: novia_gui_data.c
	$(OPTIONS) novia_gui_data.c

novia_gui.o: novia_gui.c
	$(OPTIONS) novia_gui.c

novia_reg_data.o: novia_reg_data.c
	$(OPTIONS) novia_reg_data.c

novia_gui_methods.o: novia_gui_methods.c
	$(OPTIONS) novia_gui_methods.c

novia_baseviewer.o: novia_baseviewer.c
	$(OPTIONS) novia_baseviewer.c

novia_baseview_data.o: novia_baseview_data.c
	$(OPTIONS) novia_baseview_data.c

novia_syslibrary.o: novia_syslibrary.c
	$(OPTIONS) novia_syslibrary.c

gui/novia_gui_converts.o: gui/novia_gui_converts.c
	$(OPTIONS) gui/novia_gui_converts.c

novia_nsl.o: novia_nsl.c
	$(OPTIONS) novia_nsl.c

novia_myget.o: novia_myget.c
	$(OPTIONS) novia_myget.c

novia_internal_commands.o: novia_internal_commands.c
	$(OPTIONS) novia_internal_commands.c
