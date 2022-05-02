{
        "patcher" :         {
                "fileversion" : 1,
                "appversion" :                 {
                        "major" : 7,
                        "minor" : 3,
                        "revision" : 5,
                        "architecture" : "x64",
                        "modernui" : 1
                }
,
                "rect" : [ -990.0, 77.0, 550.0, 617.0 ],
                "bglocked" : 0,
                "openinpresentation" : 0,
                "default_fontsize" : 12.0,
                "default_fontface" : 0,
                "default_fontname" : "Arial",
                "gridonopen" : 1,
                "gridsize" : [ 15.0, 15.0 ],
                "gridsnaponopen" : 1,
                "objectsnaponopen" : 1,
                "statusbarvisible" : 2,
                "toolbarvisible" : 1,
                "lefttoolbarpinned" : 0,
                "toptoolbarpinned" : 0,
                "righttoolbarpinned" : 0,
                "bottomtoolbarpinned" : 0,
                "toolbars_unpinned_last_save" : 0,
                "tallnewobj" : 0,
                "boxanimatetime" : 200,
                "enablehscroll" : 1,
                "enablevscroll" : 1,
                "devicewidth" : 0.0,
                "description" : "",
                "digest" : "",
                "tags" : "",
                "style" : "",
                "subpatcher_template" : "",
                "boxes" : [                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-2",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 223.0, 276.0, 141.0, 18.0 ],
                                        "style" : "",
                                        "text" : "Combine into one value"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-3",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 223.0, 250.0, 141.0, 18.0 ],
                                        "style" : "",
                                        "text" : "Convert to ascii"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
                                        "id" : "obj-4",
                                        "maxclass" : "number",
                                        "numinlets" : 1,
                                        "numoutlets" : 2,
                                        "outlettype" : [ "", "bang" ],
                                        "parameter_enable" : 0,
                                        "patching_rect" : [ 139.0, 304.0, 50.0, 19.0 ],
                                        "style" : "",
                                        "textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
                                        "tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
                                        "triscale" : 0.9
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-6",
                                        "maxclass" : "newobj",
                                        "numinlets" : 1,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "" ],
                                        "patching_rect" : [ 139.0, 276.0, 64.0, 19.0 ],
                                        "style" : "",
                                        "text" : "fromsymbol"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-7",
                                        "maxclass" : "newobj",
                                        "numinlets" : 3,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "int" ],
                                        "patching_rect" : [ 139.0, 250.0, 40.0, 19.0 ],
                                        "style" : "",
                                        "text" : "itoa"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-8",
                                        "maxclass" : "newobj",
                                        "numinlets" : 2,
                                        "numoutlets" : 2,
                                        "outlettype" : [ "", "" ],
                                        "patching_rect" : [ 139.0, 223.0, 61.0, 19.0 ],
                                        "style" : "",
                                        "text" : "zl group 78"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-9",
                                        "maxclass" : "newobj",
                                        "numinlets" : 3,
                                        "numoutlets" : 3,
                                        "outlettype" : [ "bang", "bang", "" ],
                                        "patching_rect" : [ 139.0, 189.0, 67.0, 19.0 ],
                                        "style" : "",
                                        "text" : "select 10 13"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "id" : "obj-12",
                                        "maxclass" : "button",
                                        "numinlets" : 1,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "bang" ],
                                        "patching_rect" : [ 315.0, 41.0, 15.0, 15.0 ],
                                        "style" : ""
                                }


                        }
,                         {
                                "box" :                                 {
                                        "id" : "obj-13",
                                        "maxclass" : "button",
                                        "numinlets" : 1,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "bang" ],
                                        "patching_rect" : [ 19.0, 119.0, 15.0, 15.0 ],
                                        "style" : ""
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-15",
                                        "linecount" : 2,
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 102.0, 70.0, 104.0, 29.0 ],
                                        "style" : "",
                                        "text" : "sample rate (15ms -- 100ms)"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
                                        "id" : "obj-16",
                                        "maxclass" : "number",
                                        "numinlets" : 1,
                                        "numoutlets" : 2,
                                        "outlettype" : [ "", "bang" ],
                                        "parameter_enable" : 0,
                                        "patching_rect" : [ 61.0, 70.0, 35.0, 19.0 ],
                                        "style" : "",
                                        "textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
                                        "tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
                                        "triscale" : 0.9
                                }


                        }
,                         {
                                "box" :                                 {
                                        "id" : "obj-17",
                                        "maxclass" : "slider",
                                        "min" : 15.0,
                                        "numinlets" : 1,
                                        "numoutlets" : 1,
                                        "orientation" : 1,
                                        "outlettype" : [ "" ],
                                        "parameter_enable" : 0,
                                        "patching_rect" : [ 61.0, 41.0, 127.0, 19.0 ],
                                        "size" : 86.0,
                                        "style" : ""
                                }


                        }
,                         {
                                "box" :                                 {
                                        "id" : "obj-18",
                                        "maxclass" : "toggle",
                                        "numinlets" : 1,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "int" ],
                                        "parameter_enable" : 0,
                                        "patching_rect" : [ 19.0, 42.0, 15.0, 15.0 ],
                                        "style" : ""
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-19",
                                        "maxclass" : "newobj",
                                        "numinlets" : 2,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "bang" ],
                                        "patching_rect" : [ 19.0, 96.0, 52.0, 19.0 ],
                                        "style" : "",
                                        "text" : "metro 10"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-20",
                                        "maxclass" : "newobj",
                                        "numinlets" : 1,
                                        "numoutlets" : 2,
                                        "outlettype" : [ "int", "" ],
                                        "patching_rect" : [ 139.0, 161.0, 94.0, 19.0 ],
                                        "style" : "",
                                        "text" : "serial e 115200 8 1 0"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 9.0,
                                        "id" : "obj-21",
                                        "maxclass" : "message",
                                        "numinlets" : 2,
                                        "numoutlets" : 1,
                                        "outlettype" : [ "" ],
                                        "patching_rect" : [ 315.0, 70.0, 32.0, 19.0 ],
                                        "style" : "",
                                        "text" : "print"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-22",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 312.0, 22.0, 86.0, 18.0 ],
                                        "style" : "",
                                        "text" : "list serial ports"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-23",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 9.0, 22.0, 47.0, 18.0 ],
                                        "style" : "",
                                        "text" : "On/Off"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-24",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 223.0, 192.0, 174.0, 18.0 ],
                                        "style" : "",
                                        "text" : "Ascii character 10 = CR, 13 = LF"
                                }


                        }
,                         {
                                "box" :                                 {
                                        "fontname" : "Arial",
                                        "fontsize" : 10.0,
                                        "id" : "obj-25",
                                        "maxclass" : "comment",
                                        "numinlets" : 1,
                                        "numoutlets" : 0,
                                        "patching_rect" : [ 223.0, 223.0, 141.0, 18.0 ],
                                        "style" : "",
                                        "text" : "Group characters until LF"
                                }


                        }
 ],
                "lines" : [                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-21", 0 ],
                                        "source" : [ "obj-12", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-20", 0 ],
                                        "midpoints" : [ 28.0, 145.0, 148.5, 145.0 ],
                                        "source" : [ "obj-13", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-19", 1 ],
                                        "source" : [ "obj-16", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-16", 0 ],
                                        "midpoints" : [ 70.5, 60.0, 70.5, 60.0 ],
                                        "source" : [ "obj-17", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-19", 0 ],
                                        "source" : [ "obj-18", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-13", 0 ],
                                        "source" : [ "obj-19", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-9", 0 ],
                                        "source" : [ "obj-20", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-20", 0 ],
                                        "midpoints" : [ 324.5, 145.0, 148.5, 145.0 ],
                                        "source" : [ "obj-21", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-4", 0 ],
                                        "source" : [ "obj-6", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-6", 0 ],
                                        "source" : [ "obj-7", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-7", 0 ],
                                        "source" : [ "obj-8", 0 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-8", 0 ],
                                        "midpoints" : [ 196.5, 215.0, 148.5, 215.0 ],
                                        "source" : [ "obj-9", 2 ]
                                }


                        }
,                         {
                                "patchline" :                                 {
                                        "destination" : [ "obj-8", 0 ],
                                        "midpoints" : [ 172.5, 215.0, 148.5, 215.0 ],
                                        "source" : [ "obj-9", 1 ]
                                }


                        }
 ],
                "dependency_cache" : [  ],
                "autosave" : 0
        }


}