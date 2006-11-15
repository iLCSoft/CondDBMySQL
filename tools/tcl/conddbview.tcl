#!/bin/sh
# The next line is a comment in WISH but not at SHELL\
exec tixwish "$0" "$@"
#
# Tool for monitoring and debug of the CondtitionsDB implementation.
# It uses the 'cdb-query' tool found in the tools directory
#
# Developed by Jorge Lima to the ATLAS-TDAQ FCUL-CERN collaboration
#  2002-08-16
#

# Set look and feel
tix resetoptions TixGray TK

# Global vars
set BINTOOL "./cdbadmin"

set INITSTR "localhost:test_conddb:jmal:online"

set FOLDERS ""
set FSETS ""
set TAGS ""
set TAGGED ""

array set CANVAS {
    win ""
    width 3000
    height 200
    voffset 0
    hoffset 0
    vscale 1
    hscale 1
    grid 1
}

array set VIEW {
    folder ""
    object ""
    tag ""
}

# Selected items
set SEL_FOLDER ""
set SEL_FSET ""
set SEL_ANYFLD ""
set SEL_TAG ""
set SEL_POINT ""
set IS_FSET 1

# Representation limits
array set LIMITS {
    lowertag 0
    uppertag 20
    since 0.0
    till 2000.0
}

array set HELP {
    Manual {
NOT YET READY
    }
    About {
Conddbview V0.1
Conditions Database Viewer
Atlas Daq Lisbon collaboration
Jorge Lima
Jorge.Lima@fc.ul.pt
    }
}
#############################################################################
# Common dialog Head and Tail                                               #
#############################################################################

proc dialogHead { top title args } {
    catch {destroy $top}
    toplevel $top
    wm transient $top .
    wm title $top $title
    eval frame $top.main -relief flat $args
    return $top.main
}

proc dialogTailOkCancel { top okcmd cancelcmd } {
    frame $top.sep -borderwidth 2 -height 2 -relief sunken
    frame $top.buttons -relief flat
    button $top.buttons.ok -text Ok -width 10 -command "$okcmd; destroy $top"
    button $top.buttons.cancel -text Cancel -width 10 -command "$cancelcmd; destroy $top"
    pack $top.buttons.ok $top.buttons.cancel -padx 15 -pady 10 -side left -expand true
    pack $top.buttons -side bottom
    pack $top.sep -side bottom -fill x -padx 40
    pack $top.main -side top -padx 10 -pady 10 -expand true
    pack propagate $top.main 0
    wm resizable $top 0 0
    update
    grab set $top
    tkwait window $top
    grab release $top
}

proc dialogTailClose { top {closecmd ""} } {
    frame $top.buttons -relief flat
    button $top.buttons.ok -text Close -width 10 -command "$closecmd; destroy $top"
    pack $top.buttons.ok -padx 30 -pady 10 -side right
    pack $top.buttons -side bottom -fill x
    pack $top.main -side top -padx 10 -pady 10 -fill both -expand true
    pack propagate $top.main 0
    update
}
 
#############################################################################
# Specific procedures                                                       #
#############################################################################

#
# Procedures to call 'cdb-query' tool
#
proc execute { command } {
    global BINTOOL INITSTR
    set options "-s$INITSTR"
    set command [join [list $BINTOOL $options $command]]
    puts $command
    if [catch { open "|$command" r } fd] {
	ErrorDialog "Error while opening pipe" "$fd"
	error "Error on \"execute\""
    }
    while { ![eof $fd] } {
	eval [gets $fd]
    }
    if [catch {close $fd} mess] {
	ErrorDialog "Error returned by \"$BINTOOL\"" "ERRO\n$mess"
	error "Error on \"execute\" while exec:\n $BINTOOL $options $command"
    }
}

#############################################################################
# Routines for iterators                                                    #
#############################################################################

proc hIterObjs { } {
    global FOLDERS TAGS SEL_FOLDER SEL_TAG
    set tmp_folder $SEL_FOLDER
    set tmp_tag $SEL_TAG
    set frm [dialogHead .box1 "Horizontal iterator" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Folder: " -variable SEL_FOLDER \
	    -options { label.width 9 label.anchor e menubutton.width 32 }
    foreach fld $FOLDERS { $frm.o1 add command $fld -label $fld }
    tixOptionMenu $frm.o2 -label "Tags: " -variable SEL_TAG \
	    -options { label.width 9 label.anchor e menubutton.width 15 }
    foreach tag $TAGS { $frm.o2 add command $tag -label $tag }
    pack $frm.o1 $frm.o2 -side top -padx 20 -pady 10
    if { $tmp_folder != "" } { set SEL_FOLDER $tmp_folder }
    if { $tmp_tag != "" } { set SEL_TAG $tmp_tag }
    dialogTailOkCancel .box1 {fillObjectView h $SEL_FOLDER $SEL_TAG} {}	
}

proc vIterObjs { } {
    global FOLDERS TAGS SEL_FOLDER SEL_POINT
    set tmp_folder $SEL_FOLDER
    set frm [dialogHead .box1 "Vertical iterator" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Folder: " -variable SEL_FOLDER \
	    -options { label.width 9 label.anchor e menubutton.width 32 }
    foreach fld $FOLDERS { $frm.o1 add command $fld -label $fld }
    if { $tmp_folder != "" } { set SEL_FOLDER $tmp_folder }
    tixControl $frm.e1 -label "Point: " -variable SEL_POINT -integer 1
    pack $frm.o1 $frm.e1 -side top -padx 20 -pady 10
    if { $tmp_folder != "" } { set SEL_FOLDER $tmp_folder }
    dialogTailOkCancel .box1 {fillObjectView v $SEL_FOLDER $SEL_POINT} {}	
}

proc aIterObjs { } {
    global FOLDERS TAGS SEL_FOLDER
    set tmp_folder $SEL_FOLDER
    set frm [dialogHead .box1 "Vertical iterator" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Folder: " -variable SEL_FOLDER \
	    -options { label.width 9 label.anchor e menubutton.width 32 }
    foreach fld $FOLDERS { $frm.o1 add command $fld -label $fld }
    if { $tmp_folder != "" } { set SEL_FOLDER $tmp_folder }
    pack $frm.o1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {fillObjectView a $SEL_FOLDER ""} {}	
}

#############################################################################
# Folder procedures                                                         #
#############################################################################

proc folderCreate { } {
    global FSETS SEL_FSET tmp_folder tmp_parent
    set frm [dialogHead .box1 "Create folder" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Parent folderSet: " -variable tmp_parent \
	    -options { label.anchor e menubutton.width 32 }
    foreach fset $FSETS { $frm.o1 add command $fset -label $fset }
    if { $SEL_FSET != "" } { set tmp_parent $SEL_FSET }
    tixLabelEntry $frm.e1 -label "New folder: "
    [$frm.e1 subwidget entry] configure -textvariable tmp_folder
    pack $frm.o1 $frm.e1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "create folder \"${tmp_parent}/${tmp_folder}\""
	readFolders
    } {}
}

proc folderTag { } {
    global TAGS SEL_TAG tmp_tag
    global FOLDERS SEL_FOLDER tmp_folder
    set frm [dialogHead .box1 "Tag folder" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Folder to tag: " -variable tmp_folder \
	    -options { label.anchor e menubutton.width 32 }
    foreach fld $FOLDERS { $frm.o1 add command $fld -label $fld }
    if { $SEL_FOLDER != "" } { set tmp_folder $SEL_FOLDER }
    tixOptionMenu $frm.o2 -label "with tag: " -variable tmp_tag \
	    -options { label.anchor e menubutton.width 32 }
    foreach tag $TAGS { $frm.o2 add command $tag -label $tag }
    if { $SEL_TAG != "" } { set tmp_tag $SEL_TAG }
    pack $frm.o1 $frm.o2 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "tag folder \"$tmp_tag\" \"$tmp_folder\""
	readFolders
    } {}
}

proc folderSetCreate { } {
    global FSETS SEL_FSET tmp_parent tmp_fset
    set frm [dialogHead .box1 "Create folderSet" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "Parent folderSet: " -variable tmp_parent \
	    -options { label.anchor e menubutton.width 32 }
    foreach fset $FSETS { $frm.o1 add command $fset -label $fset }
    if { $SEL_FSET != "" } { set tmp_parent $SEL_FSET }
    tixLabelEntry $frm.e1 -label "New folderSet: "
    [$frm.e1 subwidget entry] configure -textvariable tmp_fset
    pack $frm.o1 $frm.e1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "create fset \"${tmp_parent}/${tmp_fset}\""
	readFolders
    } {}
}

#############################################################################
# Tag procedures                                                            #
#############################################################################

proc showTaggedFolders { tag } {
    global TAGGED
    set TAGGED ""
    execute "list tagged \"$tag\""
    set frm [dialogHead .tagged "Folders tagged with $tag" -width 400 -height 200]
    tixScrolledHList $frm.a -options {hlist.columns 3 hlist.header true}
    pack $frm.a -expand yes -fill both -side left
    set hlist [$frm.a subwidget hlist]
    $hlist header create 0 -itemtype text -text "Folder"
    $hlist header create 1 -itemtype text -text "Tag date"
 #   $hlist column width 2 0
    $hlist add .
    set i 0
    foreach {fld ins} $TAGGED {
	$hlist add .it$i -itemtype text -text $fld 
	$hlist item create .it$i 1 -itemtype text -text $ins
	incr i
    }
    dialogTailClose .tagged {}
}

proc tagCreate { } {
    global tmp_tag tmp_desc
    set frm [dialogHead .box1 "Create tag" -width 300 -height 150]

    tixLabelEntry $frm.e1 -label "Tag name: "
    [$frm.e1 subwidget entry] configure -textvariable tmp_tag

    tixLabelEntry $frm.e2 -label "Description: "
    [$frm.e2 subwidget entry] configure -textvariable tmp_desc

    pack $frm.e1 $frm.e2 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "create tag \"$tmp_tag\" \"$tmp_desc\""
	readTags
    } {}
}

proc tagDelete { } {
    global TAGS SEL_TAG tmp_tag
    set frm [dialogHead .box1 "Delete tag" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "with tag: " -variable tmp_tag \
	    -options { label.anchor e menubutton.width 32 }
    foreach tag $TAGS { $frm.o1 add command $tag -label $tag }
    if { $SEL_TAG != "" } { set tmp_tag $SEL_TAG }
    pack $frm.o1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "tag delete \"$tmp_tag\""
	readTags
    } {}
}

proc tagRename { } {
    global TAGS SEL_TAG tmp_oldtag tmp_newtag
    set frm [dialogHead .box1 "Rename tag" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "with tag: " -variable tmp_oldtag \
	    -options { label.anchor e menubutton.width 32 }
    foreach tag $TAGS { $frm.o1 add command $tag -label $tag }
    if { $SEL_TAG != "" } { set tmp_oldtag $SEL_TAG }
    tixLabelEntry $frm.e1 -label "Tag name: "
    [$frm.e1 subwidget entry] configure -textvariable tmp_newtag
    pack $frm.o1 $frm.e1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	execute "tag rename \"$tmp_oldtag\" \"$tmp_newtag\""
	readTags
    } {}
}

proc tagListFolders { } {
    global TAGS SEL_TAG tmp_tag
    set frm [dialogHead .box1 "List tagged folders" -width 300 -height 150]
    tixOptionMenu $frm.o1 -label "with tag: " -variable tmp_tag \
	    -options { label.anchor e menubutton.width 32 }
    foreach tag $TAGS { $frm.o1 add command $tag -label $tag }
    if { $SEL_TAG != "" } { set tmp_tag $SEL_TAG }
    pack $frm.o1 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {showTaggedFolders $tmp_tag} {}
}

#############################################################################
# Object view                                                               #
#############################################################################

proc createObjectView { widget } {
    global CANVAS
    set f [frame $widget.f]
    canvas $f.c -width 400 -bg white \
	    -xscrollcommand "$f.h set" -yscrollcommand "$f.v set"
    scrollbar $f.v -orient vertical -command "$f.c yview"
    scrollbar $f.h -orient horizontal -command "$f.c xview"
    grid $f.c -column 0 -row 0 -sticky nswe
    grid $f.v -column 1 -row 0 -sticky ns 
    grid $f.h -column 0 -row 1 -sticky ew 
    grid columnconfigure $f 0 -weight 1
    grid rowconfigure $f 0 -weight 1
    set CANVAS(win) $f.c
    return $f
}

proc fillObjectView { type folder {arg ""} } {
    global CANVAS LIMITS
    deleteObjects
    setvrange $LIMITS(lowertag) $LIMITS(uppertag)
    sethrange $LIMITS(since) $LIMITS(till)
    drawAxis $CANVAS(width) $CANVAS(height)
  
    switch $type {
	"a" {
	    execute "list objects $type \"$folder\""
	}
	"v" {
	    set point $arg
	    execute "list objects $type \"$folder\" $point"
	}
	"h" {
	    set tag $arg
	    execute "list objects $type \"$folder\" \"$tag\""
	}
	default {error "BUG in fillObjectView caller"}
    }
}

# Maniputate object view
proc deleteObjects {} {
    global CANVAS
    $CANVAS(win) delete all
}

proc drawAxis { width height } {
    global CANVAS LIMITS
    set margin 80
    set y1 [expr -$height-$margin]
    set y2 $margin
    set x1 -$margin
    set x2 [expr $width+$margin]
    set ax [expr $width+30]
    set ay [expr $height+30]
    for { set i 0 } { $i < 21 } { incr i } {
	set y [expr -$i*$CANVAS(vscale)]
	$CANVAS(win) create line "-5 $y 0 $y"
	if { $CANVAS(grid) } {
	    $CANVAS(win) create line "0 $y $ax $y" -fill grey90
	}
    }
    set range [expr $LIMITS(till)-$LIMITS(since)]
    set t1 [expr $range/30.0]
    for { set i 0 } { $i < 31 } { incr i } {
	set x [expr $i*$t1*$CANVAS(hscale)]
	$CANVAS(win) create line "$x 0 $x 5"
	if { $CANVAS(grid) } {
	    $CANVAS(win) create line "$x 0 $x -$ay" -fill lightblue
	}
    }
    $CANVAS(win) create line "-10 0 $ax 0" -arrow last
    $CANVAS(win) create line "0 10 0 -$ay" -arrow last
    $CANVAS(win) configure -scrollregion "$x1 $y1 $x2 $y2"
    set CANVAS(width) $width
    set CANVAS(height) $height
}

proc sethrange { since till } {
    global CANVAS
    set CANVAS(hscale) [expr $CANVAS(width)*1.0/($till-$since)]
    set CANVAS(hoffset) [expr -$CANVAS(hscale)*$since]
    puts "$CANVAS(hscale) $CANVAS(hoffset)"
}

proc setvrange { lowerlayer upperlayer } {
    global CANVAS
    set CANVAS(vscale) 10
    set CANVAS(voffset) [expr $CANVAS(vscale)*$lowerlayer]
    set range [expr ($upperlayer-$lowerlayer)*$CANVAS(vscale)]
    if { $range > $CANVAS(height) } {
	set CANVAS(height) $range
    }
}

proc drawobject { since till layer color props } {
    global CANVAS
    set scale $CANVAS(hscale)
    set offset $CANVAS(hoffset)
    set x1 [expr $since*$scale+$offset]
    set x2 [expr $till*$scale+$offset]
    set scale $CANVAS(vscale)
    set offset $CANVAS(voffset)
    set y1 [expr -($layer+1)*$scale-$offset]
    set y2 [expr -$layer*$scale-$offset]
    set id [$CANVAS(win) create rectangle "$x1 $y1 $x2 $y2" -fill $color]
    $CANVAS(win) bind $id <Button-3> "openInfoPopup %W %x %y \"$props\""
    $CANVAS(win) bind $id <ButtonRelease-3> "closeInfoPopup %W"
}

proc openInfoPopup { w x y props } {
    catch { destroy $w.infoPopup }
    frame $w.infoPopup -bg black -borderwidth 1
    set n 1
    foreach {label val} $props {
	label $w.infoPopup.l$n -text $label -bg yellow -anchor e
	label $w.infoPopup.v$n -text $val -bg yellow -anchor w
	grid $w.infoPopup.l$n -row $n -column 0 -sticky nsew
	grid $w.infoPopup.v$n -row $n -column 1 -sticky nsew
	incr n
    }
    place $w.infoPopup -x $x -y $y
}

proc closeInfoPopup { w } {
    catch { destroy $w.infoPopup }
}

proc SetupView {} {
    global CANVAS LIMITS TMP_SINCE TMP_TILL
    set TMP_SINCE $LIMITS(since)
    set TMP_TILL $LIMITS(till)
    set frm [dialogHead .box1 "Visualization limits" -width 300 -height 150]
    tixLabelEntry $frm.e1 -label "Since: "
    [$frm.e1 subwidget entry] configure -textvariable TMP_SINCE
    tixLabelEntry $frm.e2 -label "Till: "
    [$frm.e2 subwidget entry] configure -textvariable TMP_TILL
    pack $frm.e1 $frm.e2 -side top -padx 20 -pady 10
    dialogTailOkCancel .box1 {
	set LIMITS(since) $TMP_SINCE
	set LIMITS(till) $TMP_TILL
    } {}
}

#############################################################################
# Folder view                                                               #
#############################################################################

proc createFolderView { widget } {
    proc openFolderPopup { x y } {
	global IS_FSET
	if { $IS_FSET } {
	    tk_popup .folderSetPopup $x $y
	} else {
	    tk_popup .folderPopup $x $y
	}	    
    }
    menu .folderSetPopup -tearoff 0
    .folderSetPopup add command -label "Add folder" -command folderCreate
    .folderSetPopup add command -label "Add folderSet" -command folderSetCreate
    .folderSetPopup add command -label "Delete folderSet" -command folderDelete
    menu .folderPopup -tearoff 0
    .folderPopup add command -label "Tag folder" -command folderTag
    .folderPopup add command -label "Delete folder" -command folderDelete
    tixTree $widget.v -options { separator "/" } -browsecmd selectDir
    bind [$widget.v subwidget hlist] <Button-3> {openFolderPopup %X %Y}
    return $widget.v
}

proc fillFolderView { treeWidget } {
    global FSETS FOLDERS
    set hlist [$treeWidget subwidget hlist]
    $hlist delete all
    foreach d [lsort $FSETS] {
	set text [lindex [split $d /] end]
	$hlist add $d -itemtype imagetext -text $text -image [tix getimage folder]
    }
    foreach d $FOLDERS {
	set text [lindex [split $d /] end]
	$hlist add $d -itemtype imagetext -text $text -image [tix getimage file]
    }
    $treeWidget autosetmode
}

proc selectDir { entry } {
    global SEL_FOLDER SEL_FSET SEL_ANYFLD
    global FOLDERS FSETS IS_FSET
    set SEL_ANYFLD $entry
    if { [lsearch $FOLDERS $entry]>=0 } {
	set SEL_FOLDER $entry
	set IS_FSET 0
    } else {
	set SEL_FSET $entry
	set IS_FSET 1
    }
}

proc readFolders {} {
    global VIEW FOLDERS FSETS
    set FSETS ""
    set FOLDERS ""
    execute "list fsets"
    execute "list folders"
    fillFolderView $VIEW(folder)
}


#############################################################################
# Tag view                                                                  #
#############################################################################

proc createTagView { widget } {
    menu .tagPopup -tearoff 0
    .tagPopup add command -label "Create tag" -command tagCreate
    .tagPopup add command -label "Delete tag" -command tagDelete
    .tagPopup add command -label "Rename tag" -command tagRename
    .tagPopup add separator
    .tagPopup add command -label "List tagged folders" -command tagListFolders
    tixTree $widget.v -browsecmd selectTag
    bind [$widget.v subwidget hlist] <Button-3> {tk_popup .tagPopup %X %Y}
    return $widget.v
}

proc fillTagView { tagWidget } {
    global TAGS
    set hlist [$tagWidget subwidget hlist]
    $hlist delete all
    foreach t $TAGS {
	$hlist add $t -text $t
    }
}

proc selectTag { entry } {
    global SEL_TAG
    set SEL_TAG $entry
}

proc readTags {} {
    global VIEW TAGS
    set TAGS ""
    execute "list tags"
    fillTagView $VIEW(tag)
}

#############################################################################
# Callbacks                                                                 #
#############################################################################

proc object { layer since till descr } {
    global CANVAS LIMITS
    set SINCE $LIMITS(since)
    set TILL $LIMITS(till)
    set props [list Since: $since Till: $till Layer: $layer Description: $descr]
    if { $since>$TILL || $till<$SINCE } return
    if { $till>$TILL || $till=="+Inf" } { set till $TILL }
    if { $since<$SINCE || $since=="-Inf" } { set since $SINCE }
    if { $descr=="Empty object" } {
	drawobject $since $till $layer grey90 $props
    } else {
	drawobject $since $till $layer grey60 $props
    }
}

proc folder { foldername } {
    global FOLDERS
    lappend FOLDERS $foldername
}

proc folderset { foldername } {
    global FSETS
    lappend FSETS $foldername
}

proc tag { tagname } {
    global TAGS
    lappend TAGS $tagname
}

proc tagged { folder instime } {
    global TAGGED
    lappend TAGGED $folder $instime
}

#############################################################################
# Other procedures                                                          #
#############################################################################

proc ErrorDialog { s1 s2 } {
    tk_messageBox -icon error -parent . -message $s1 -type ok -title Error
    puts $s2
}

proc Exit {} {
    set res [tk_messageBox -icon question -parent . \
	    -message "Realy quit?" -type yesno -title Confirm]
    if { $res=="yes" } { exit }
}

proc HelpWin { what } {
    global HELP
    set frm [dialogHead .helpbox "$what" -width 400 -height 250]
    text $frm.txt
    pack $frm.txt -side top -fill both -padx 10
    $frm.txt insert end $HELP($what)
    dialogTailClose .helpbox {}    
}

proc OpenDB {} {
    global VIEW
    # Open database
    # ...
    # Fill folder and tag views
    if [catch {readFolders; readTags} mess] {
	ErrorDialog "Database server connection failed!" "$mess"
	return
    }
    # Activate menus
    .m.file entryconfigure "Open database" -state disabled
    .m.file entryconfigure "Close database" -state normal
    .m.action entryconfigure "Iterate horizontaly" -state normal
    .m.action entryconfigure "Iterate verticaly" -state normal
    .m.action entryconfigure "Iterate all in folder" -state normal
    .m.view entryconfigure "Visualization limits" -state normal
    .m.view entryconfigure "Update folders" -state normal
    .m.view entryconfigure "Update tags" -state normal
}

proc CloseDB {} {
    global VIEW FOLDERS FSETS TAGS
    set FOLDERS ""
    set FSETS ""
    set TAGS ""
    # Fill folder and tag views
    fillFolderView $VIEW(folder)
    fillTagView $VIEW(tag)
    # Activate menus
    .m.file entryconfigure "Open database" -state normal
    .m.file entryconfigure "Close database" -state disabled
    .m.action entryconfigure "Iterate horizontaly" -state disabled
    .m.action entryconfigure "Iterate verticaly" -state disabled
    .m.action entryconfigure "Iterate all in folder" -state disabled
    .m.view entryconfigure "Visualization limits" -state disabled
    .m.view entryconfigure "Update folders" -state disabled
    .m.view entryconfigure "Update tags" -state disabled
}

#############################################################################
# Procedures to build GUI                                                   #
#############################################################################

proc createViews { } {
    global VIEW
    set wa [tixPanedWindow .workarea -orient horizontal]
    set pl [$wa add paneLeft]
    set pr [$wa add paneRight]
    set lwa [tixPanedWindow $pl.wa -orient vertical -width 200]
    set lpu [$lwa add paneUp]
    set lpd [$lwa add paneDown]
    pack $lwa -fill both -expand yes
    set VIEW(folder) [createFolderView $lpu]
    pack $VIEW(folder) -fill both -expand yes -padx 5 -pady 5
    set VIEW(tag) [createTagView $lpd]
    pack $VIEW(tag) -fill both -expand yes -padx 5 -pady 5 
    set VIEW(object) [createObjectView $pr]
    pack $VIEW(object) -fill both -expand yes -padx 5 -pady 5
    pack $wa -fill both -expand yes
}

proc createMenus {} {
    menu .m -tearoff 0
    menu .m.file
    .m.file add command -label "Open database" -command OpenDB
    .m.file add command -label "Close database" -command CloseDB -state disabled
    .m.file add separator
    .m.file add command -label "Exit" -command Exit
    .m add cascade -label "File" -menu .m.file
    menu .m.action
    .m.action add command -label "Iterate horizontaly" -command hIterObjs -state disabled
    .m.action add command -label "Iterate verticaly" -command vIterObjs -state disabled
    .m.action add command -label "Iterate all in folder" -command aIterObjs -state disabled
    .m add cascade -label "Actions" -menu .m.action
    menu .m.view
    .m.view add command -label "Visualization limits" -command SetupView -state disabled
    .m.view add separator
    .m.view add command -label "Update folders" -command readFolders -state disabled
    .m.view add command -label "Update tags" -command readTags -state disabled
    .m add cascade -label "View" -menu .m.view
    menu .m.help -tearoff 0
    .m.help add command -label "Manual" -command "HelpWin Manual"
    .m.help add command -label "About" -command "HelpWin About"
    .m add cascade -label "Help" -menu .m.help
    . configure -menu .m
}


proc createStatusBar {} {
    frame .bottom
    label .bottom.l1 -relief sunken -width 22 -borderwidth 1 -textvariable SEL_ANYFLD
    label .bottom.l2 -relief sunken -width 22 -borderwidth 1 -textvariable SEL_TAG
    pack .bottom.l1 .bottom.l2 -padx 8 -pady 5 -side left
    pack .bottom -side bottom -fill x -expand no
}

#
# Build GUI
#
createStatusBar
createViews
createMenus

wm protocol . WM_DELETE_WINDOW Exit
#
# THE END
#
















