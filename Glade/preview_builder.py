#!/usr/bin/python
# *-* coding: utf-8 *-*

import sys
import pygtk
#pygtk.require("2.0")
import gtk
import gtk.glade
import fnmatch
import os
import sys
import re
import glob

sys.path.append('/incam/server/site_data/scripts/python')
import incam



class MainWindow:
    def __init__(self):
        self.gladefile="./demo_builder.glade"
        self.builder = gtk.Builder()
        self.builder.add_from_file(self.gladefile)
        self.builder.connect_signals(self)
        self.windows1 = self.builder.get_object("window1")
        self.f = incam.InCAM()
        self.windows1.set_position(gtk.WIN_POS_CENTER)
        self.windows1.show()
    def on_window1_destroy(self,object):
        gtk.main_quit()
    def on_exit_clicked(self,object):
        gtk.main_quit()
    def on_window1_show(self, widget):
        info = self.f.DO_INFO('-t matrix -e ' + os.environ['JOB'] + '/matrix')
        self.step = self.builder.get_object("step")
        self.store_step = self.create_store(info['gCOLstep_name'])
        self.step.set_model(self.store_step)
        self.step.set_active(0)
# And here's the new stuff:
        cell = gtk.CellRendererText()
        self.step.pack_start(cell, True)
        self.step.add_attribute(cell, "text", 0)


        self.store_vl = self.create_store(info['gROWname'])
        self.layer_vl = gtk.TreeViewColumn()
        self.cell_vl = gtk.CellRendererText()
        self.layer_vl.pack_start(self.cell_vl,True)
        self.layer_vl.add_attribute(self.cell_vl,"text",0)
        #self.layer_vl.set_title("Versions")
        layer_list = self.builder.get_object("layer_list")
        layer_list.set_headers_visible(False)
        #self.layer_vl.set_sort_column_id(0)
        layer_list.get_selection().set_mode(gtk.SELECTION_MULTIPLE)
        layer_list.set_rubber_banding(True)
        layer_list.append_column(self.layer_vl)
        layer_list.set_model(self.store_vl)

    def on_compare_clicked(self,widget):
        stepName = self.step.get_active_text()
        tolValue = self.builder.get_object("tol").get_text()
        resValue = self.builder.get_object("res").get_text()
        layer_list = self.builder.get_object("layer_list")
        layers_selected = layer_list.get_selection()
        (model,pathlist) = layers_selected.get_selected_rows()

        for path in pathlist:
            tree_iter = model.get_iter(path)
            layer = model.get_value(tree_iter,0)
            self.f.COM("compare_layers,layer1="+layer+",job2="+os.environ['JOB']+",step2="+stepName+",layer2="+layer+",layer2_ext=,tol="+tolValue+",area=global,consider_sr=yes,ignore_attr=,map_layer="+layer+"_tmp,map_layer_res="+resValue)


    def create_store(self,list):
        store = gtk.ListStore(str)
        #info = self.f.DO_INFO('-t matrix -e ' + os.environ['JOB'] + '/matrix')

        for layer in list:
            store.append([layer])
        return store

if __name__ == "__main__":
    mw = MainWindow()
    gtk.main()

