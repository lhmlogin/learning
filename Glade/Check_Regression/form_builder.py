#!/usr/bin/python
# *-* coding: utf-8 *-*

import sys
import pygtk
pygtk.require("2.0")
import gtk
import gtk.glade
import fnmatch
import os
import sys
import re
import glob

os.environ['FRONTLINE_NO_LOGIN_SCREEN']='/auto_incam/server/users/billy'
script_path = "/workfile/maintain_scripts/check_regression_version/"

class MainWindow:
    def __init__(self):
        self.gladefile="gui_builder.xml"
        self.builder = gtk.Builder()
        self.builder.add_from_file(self.gladefile)
        self.builder.connect_signals(self)
        self.form = self.builder.get_object("form")
        self.form.set_position(gtk.WIN_POS_CENTER)
        self.form.show()
    def on_form_destroy(self,widget):
        gtk.main_quit()

    def on_exit_clicked(self,widget):
        gtk.main_quit()

    def on_form_show(self, widget):
        #add versions to version list
        self.store_vl = self.create_store()
        self.version_vl = gtk.TreeViewColumn()
        self.cell_vl = gtk.CellRendererText()
        self.version_vl.pack_start(self.cell_vl,True)
        self.version_vl.add_attribute(self.cell_vl,"text",0)
        self.version_vl.set_title("Versions")
        treeview_versionlist = self.builder.get_object("treeview_version_list")
        #treeview_versionlist.set_headers_visible(False)
        self.version_vl.set_sort_column_id(0)
        treeview_versionlist.get_selection().set_mode(gtk.SELECTION_MULTIPLE)
        treeview_versionlist.set_rubber_banding(True)
        treeview_versionlist.append_column(self.version_vl)
        treeview_versionlist.set_model(self.store_vl)

        self.store_vc = gtk.ListStore(str)
        treeview_compare_version = self.builder.get_object("treeview_compare_version")
        self.version_vc = gtk.TreeViewColumn()
        self.cell_vc = gtk.CellRendererText()
        self.version_vc.pack_start(self.cell_vc,True)
        self.version_vc.add_attribute(self.cell_vc,"text",0)
        treeview_compare_version.set_headers_visible(False)
        treeview_compare_version.append_column(self.version_vc)
        treeview_compare_version.set_model(self.store_vc)

    def on_combo_from_changed(self, widget):
        #if the value is file, show the hbox_file, else hide the hbox_file
        combo_from = self.builder.get_object("combo_from")
        combo_from_name = combo_from.get_active_text()
        if combo_from_name == "file":
            #self.builder.get_object("hbox_file").show()
            self.builder.get_object("hbox_file").show()
        else:
            #self.builder.get_object("hbox_file").hide()
            self.builder.get_object("hbox_file").hide()

    def on_combo_mode_changed(self, widget):
        #change the label name just below it
        combo_mode = self.builder.get_object("combo_mode")
        combo_mode_index = combo_mode.get_active()
        if combo_mode_index == 0:
            self.builder.get_object("label_num").show()
            self.builder.get_object("label_x").hide()
            self.builder.get_object("label_mark").hide()
            self.builder.get_object("label_on").show()
            self.builder.get_object("label_y").hide()
            self.builder.get_object("ent_y").show()
        elif combo_mode_index == 1:
            self.builder.get_object("label_num").hide()
            self.builder.get_object("label_x").show()
            self.builder.get_object("label_mark").hide()
            self.builder.get_object("label_on").hide()
            self.builder.get_object("label_y").show()
            self.builder.get_object("ent_y").show()
        elif combo_mode_index == 2:
            self.builder.get_object("label_num").hide()
            self.builder.get_object("label_x").hide()
            self.builder.get_object("label_mark").show()
            self.builder.get_object("label_on").hide()
            self.builder.get_object("label_y").hide()
            self.builder.get_object("ent_y").hide()
        else:
            self.builder.get_object("label_num").show()
            self.builder.get_object("label_x").hide()
            self.builder.get_object("label_mark").hide()
            self.builder.get_object("label_on").show()
            self.builder.get_object("label_y").hide()
            self.builder.get_object("ent_y").show()
        
    def on_button_select_clicked(self, widget):
        #popup a file select window, and set the value to ent_file
        fcd = gtk.FileChooserDialog(action=gtk.FILE_CHOOSER_ACTION_OPEN,buttons=('Cancel',1,'Open',0))
        response = fcd.run()
        if response == 0:
            filename = fcd.get_filename()
            fcd.destroy()
        else:
            filename = ''
            fcd.destroy()
        self.builder.get_object("ent_file").set_text(filename)

    def on_ent_filter_activate(self, widget):
        #filter the version list
        self.version_filter=self.store_vl.filter_new()
        self.filter_text=self.builder.get_object("ent_filter").get_text()
        self.version_filter.set_visible_func(self.filter,self.filter_text)
        treeview_versionlist = self.builder.get_object("treeview_version_list")
        treeview_versionlist.set_model(self.version_filter)
        self.version_filter.refilter()

    def filter(self,model,iter,data=None):
        if data == '':
            return True
        else:
            return fnmatch.fnmatch(model[iter][0],data)

    def on_button_right1_clicked(self, widget):
        #add the value which selected at version list to ent_current_version
        treeview_versionlist = self.builder.get_object("treeview_version_list")
        tree_selection = treeview_versionlist.get_selection()
        (model,pathlist) = tree_selection.get_selected_rows()
        if len(pathlist) == 1:
            tree_iter = model.get_iter(pathlist[0])
            value = model.get_value(tree_iter,0)
            self.builder.get_object("ent_current_version").set_text(value)
        else:
            self.raise_alert("Only one row can be selected")
            #print "only one row can be selected"
        
    def on_button_right2_clicked(self, widget):
        #add the values which selected at version list to treeview_compare_version
        
        treeview_versionlist = self.builder.get_object("treeview_version_list")
        tree_selection = treeview_versionlist.get_selection()
        (model,pathlist) = tree_selection.get_selected_rows()

        for path in pathlist:
            tree_iter = model.get_iter(path)
            value = model.get_value(tree_iter,0)
            self.store_vc.append([value])

    def on_button_delete_clicked(self, widget):
        #delete the select compare version
        treeview_compare_version = self.builder.get_object("treeview_compare_version")
        tree_selection = treeview_compare_version.get_selection()
        (model,pathlist) = tree_selection.get_selected_rows()
        for path in pathlist:
            tree_iter = model.get_iter(path)
            self.store_vc.remove(tree_iter)

    def on_submit_clicked(self, widget):
        self.get_information()
        self.which_incam()
        self.unpack_incam()
        self.run_current_incam()
        self.run_compare_incam()
        self.show_compare_result()

    def on_manual_add_clicked(self,widget):
        manual_version_text=self.builder.get_object("manual_version").get_text()
        self.store_vc.append([manual_version_text])

    def create_store(self):
        store = gtk.ListStore(str)

        #f = open("./versionlist.txt","r")
        #for line in f.readlines():
        #    version = line.strip()
        #    if version != '':
        #        store.append([version])
        #f.close()
        os.chdir('/versions/incam')
        versions=glob.glob('*InCAM*Linux64*')
        for version in versions:
            m = re.search('InCAM\.(.+)\.Linux64\.tar\.gz',version)
            if m:
                store.append([m.group(1)])
        return store
    def raise_alert(self,text):
        md = gtk.MessageDialog(None,gtk.DIALOG_DESTROY_WITH_PARENT,gtk.MESSAGE_WARNING,gtk.BUTTONS_CLOSE,text)
        md.run()
        md.destroy()

    def get_information(self):
        self.ent_job_text=self.builder.get_object("ent_job").get_text()
        self.ent_step_text=self.builder.get_object("ent_step").get_text()
        self.ent_chk_text=self.builder.get_object("ent_chk").get_text()
        self.ent_layer_text=self.builder.get_object("ent_layer").get_text()
        self.combo_from_text=self.builder.get_object("combo_from").get_active_text()
        self.combo_mode_text=self.builder.get_object("combo_mode").get_active_text()
        self.ent_x_text=self.builder.get_object("ent_x").get_text()
        self.ent_y_text=self.builder.get_object("ent_y").get_text()
        self.ent_file_text=self.builder.get_object("ent_file").get_text()
        self.ent_current_version_text=self.builder.get_object("ent_current_version").get_text()


#        print self.ent_job_text,self.ent_step_text,self.ent_chk_text,self.ent_layer_text,self.combo_from_text,self.combo_mode_text,self.ent_x_text,self.ent_y_text,self.ent_file_text,self.ent_current_version_text

        self.compare_version_list = list()
        item = self.store_vc.get_iter_first()

        while ( item != None ):
            self.compare_version_list.append(self.store_vc.get_value(item, 0))
            item = self.store_vc.iter_next(item)

#        if (self.ent_job_text==None):
#            raise_alert("please type the job name")
#        elif (self.ent_step_text==None):
#            raise_alert("please type the step name")
#        elif (self.ent_chk_text==None):
#            raise_alert("please type the checklist name")

        #print self.compare_version_list

    def which_incam(self):
        if (self.combo_from_text == "auto_incam"):
            self.incam_path = "/auto_incam/"
        elif (self.combo_from_text == "test_incam"):
            self.incam_path = "/test_incam/"
        elif (self.combo_from_text == "file"):
            computer = os.uname()[1]
            if (computer == "rhel6"):
                self.incam_path = "/frontline/testins/"
            elif (computer == "rhel65"):
                self.incam_path = "/frontline/incam/"
            elif (computer == "rhel65-coline"):
                self.incam_path = "/incam/"
            else:
                #test
                self.incam_path = "/InCAM/"
                pass
        else:
            pass
        #print self.incam_path
        

    def unpack_incam(self):
        self.incam_list = os.listdir(self.incam_path)
        app_incam_list = list(self.compare_version_list)
        app_incam_list.append(self.ent_current_version_text)

        #print app_incam_list
        for app_incam_version in app_incam_list:
            exist_flag = 0
            m = re.search('.*(\d{6})',app_incam_version)
            incam_version_1 = ""
            incam_version_2 = ""
            if m:
                incam_version_1 = m.group(1)
            #print incam_version_1
            for dir_incam_version in self.incam_list:
                m = re.search('.*(\d{6}).*',dir_incam_version)
                if m:
                    incam_version_2 = m.group(1)
                #print incam_version_2
                if (incam_version_1 == incam_version_2):
                    if(incam_version_1 != ""):
                        exist_flag = 1
            if (exist_flag == 0):
                #mark firstly to debug
                version_word_list = app_incam_version.split('.')
                if len(version_word_list) == 3:
                    app_incam_version_tmp = 'v'+version_word_list[0]+version_word_list[1]+'.'+version_word_list[2]
                elif len(version_word_list) == 2:
                    app_incam_version_tmp = version_word_list[0]+'.'+version_word_list[1]
                zip_package_full_name = "/versions/incam/InCAM."+app_incam_version+".Linux64.tar.gz"
                os.system("tar -xvzf " + zip_package_full_name + " -C " + self.incam_path)
                os.rename(self.incam_path+"release_64",self.incam_path+app_incam_version_tmp)
                print "extract " + zip_package_full_name + " zip to self.incam_path"

    def run_current_incam(self):
        self.incam_list = os.listdir(self.incam_path)
        m = re.search('.*(\d{6})',self.ent_current_version_text)
        if m:
            current_version_number = m.group(1)
        for dir_incam_version in self.incam_list:
            m = re.search('.*(\d{6})$',dir_incam_version)
            if m:
                dir_incam_version_number = m.group(1)
            else:
                dir_incam_version_number = None
            if (current_version_number == dir_incam_version_number):
                current_version_partial_path = dir_incam_version
                break
        current_version_full_path = self.incam_path + current_version_partial_path
        if (self.combo_from_text == "auto_incam" or self.combo_from_text == "test_incam"):
            os.system(current_version_full_path + "/bin/incam.csh -x -s" + script_path + "create_ref.csh "+self.ent_job_text+' '+self.ent_step_text+' '+self.ent_chk_text+' '+self.ent_layer_text)
        elif (self.combo_from_text == "file"):
            os.system(current_version_full_path + "/bin/incam.csh -x -s" + script_path + "create_ref_with_import.csh "+self.ent_job_text+' '+self.ent_step_text+' '+self.ent_chk_text+' '+self.ent_layer_text+' '+self.ent_file_text)

    def run_compare_incam(self):
        #print self.compare_version_list
        for compare_version in self.compare_version_list:
            m = re.search('.*(\d{6})',compare_version)
            if m:
                compare_version_number = m.group(1)
            for dir_incam_version in self.incam_list:
                m = re.search('.*(\d{6})$',dir_incam_version)
                if m:
                    dir_incam_version_number = m.group(1)
                else:
                    dir_incam_version_number = None
                if (compare_version_number == dir_incam_version_number):
                    compare_version_partial_path = dir_incam_version
                    break
            compare_version_full_path = self.incam_path + compare_version_partial_path
            #print "*************"
            #print compare_version_full_path
            #print "*************"
            if (self.combo_mode_text == "Note"):
                os.system(compare_version_full_path + "/bin/incam.csh -s" + script_path + "snapshot.csh "+self.ent_job_text+' '+self.ent_step_text+' '+self.ent_chk_text+' '+self.ent_layer_text+' '+self.ent_x_text+' '+self.ent_y_text+' '+self.ent_current_version_text+' '+compare_version)
        os.system(compare_version_full_path + "/bin/incam.csh -x -s" + script_path + "del_layer.csh "+self.ent_job_text+' '+self.ent_step_text+' '+self.ent_layer_text)

    def show_compare_result(self):
        ImageShower(self.ent_job_text,self.compare_version_list,self.ent_x_text)

class ImageShower:

    def __init__(self,jobname,compare_version_list,notes):
        notes_list=notes.split(',')
        
        window = gtk.Dialog()
        window.connect("destroy", self.destroy_imageshower)
        window.set_border_width(10)
        window.set_title("images: Red is current result")
        window.set_size_request(800,600)
        
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.set_border_width(10)
        scrolled_window.set_policy(gtk.POLICY_AUTOMATIC,gtk.POLICY_ALWAYS)
        window.vbox.pack_start(scrolled_window, True, True, 0)
        
        table = gtk.Table(len(compare_version_list)+1,len(notes_list)+1,False)
        table.set_border_width(5)
        table.set_row_spacings(5)
        table.set_col_spacings(10)
        scrolled_window.add_with_viewport(table)
        
#         label=gtk.Label("compare_versions\\notes")
#         table.attach(label,0,1,0,1)
#         
#         for i in range(len(compare_version_list)):
#             label=gtk.Label(compare_version_list[i])
#             table.attach(label,0,1,i+1,i+2)
#         for j in range(len(notes_list)):
#             label=gtk.Label('note'+notes_list[j])
#             table.attach(label,j+1,j+2,0,1)
#         for i in range(len(compare_version_list)):
#             for j in range(len(notes_list)):
#                 pixbuf=gtk.gdk.pixbuf_new_from_file_at_size(script_path+'snapshot/'+'/'+jobname+'/'+compare_version_list[i]+'/'+notes_list[j]+'.png',250,250)
#                 image = gtk.Image()
#                 image.set_from_pixbuf(pixbuf)
#                 table.attach(image,j+1,j+2,i+1,i+2)

        for i in range(len(compare_version_list)):
            for j in range(len(notes_list)):
                table_vbox=gtk.VBox(False)
                label=gtk.Label('note'+notes_list[j]+' of '+compare_version_list[i])
                pixbuf=gtk.gdk.pixbuf_new_from_file_at_size(script_path+'snapshot/'+'/'+jobname+'/'+compare_version_list[i]+'/'+notes_list[j]+'.png',350,350)
                image = gtk.Image()
                image.set_from_pixbuf(pixbuf)
                table_vbox.pack_start(image,True,True)
                table_vbox.pack_start(label,False,True)
                table.attach(table_vbox,j,j+1,i,i+1)

                
        window.show_all()
        
    def destroy_imageshower(self, widget):
        widget.destroy

if __name__ == "__main__":
    mw = MainWindow()
    gtk.main()

