<?php
////////////////////////////////////////////////////////////////////////
/*

Utility class for generating forms


For the lastest version go to:
http://www.phpclasses.org/browse.html/package/931.html



////////////////////////////////////////////////////////////////////////

function openForm($action, $method = 'post', $target = '', $fileUpload = false, $charset = '', $additional = '')
function closeForm()
function openFieldset($title, $pre = '')
function closeFieldset()
function hidden($name, $value='')
function checkBox($value, $group='', $selected=false, $additional='')
function checkBoxes($array, $group='', $selectArray, $additional='')
function radioButtons($array, $group='', $selectId=1, $additional = '')
function radioButton($value, $group='', $selected=false, $additional='')
function selectBox($array, $selected=0, $name='', $size=1, $multiple=false, $additional='')
function submitButton($title = '', $new = false, $newContent = '', $additional='')
function resetButton($title = '', $new = false, $newContent = '', $additional='')
function button($title, $name = '', $new = false, $newContent = '', $additional = '')
function textField($value, $name='', $hidden=false, $size =-1, $length =-1, $additional='')
function fileField($name = '', $file = '', $fileSize = 1000000, $size = -1, $accept="text/*", $additional='')
function textArea($value, $name='', $cols=-1, $rows=-1, $wrap='soft' ,$readOnly=false, $additional='')

////////////////////////////////////////////////////////////////

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
    
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
////////////////////////////////////////////////////////////////////////
/**
* @author	    Lennart Groetzbach <lennartg@web.de>
* @copyright	Lennart Groetzbach <lennartg@web.de> - distributed under the LGPL
* @version 	    0.5 - 2002/12/30
*/

class Forms {

////////////////////////////////////////////////////////////////////////

function openForm($action, $method = 'post', $target = '', $fileUpload = false, $charset = '', $additional = '') {
    $res = '';
    $res .= "<form action=\"$action\" method=\"$method\"";
    $res .= ($fileUpload == true ? " enctype=\"multipart/form-data\"" : '');
    $res .= ($charset == '' ? '' : " accept-charset=\"$charset\"");
    $res .= ($target == '' ? '' : " target=\"$target\"");
    $res .= ($additional ? " $additional" : '') . '>';
    return $res;
}

////////////////////////////////////////////////////////////////////////

function closeForm() {
    return "</form>";
}

////////////////////////////////////////////////////////////////////////

function openFieldset($title, $pre = '') {
    return "<fieldset><legend>$pre$title</legend>";
}

////////////////////////////////////////////////////////////////////////

function closeFieldset() {
    return "</fieldset>";
}

////////////////////////////////////////////////////////////////////////

function hidden($name, $value='') {
    return "<input type=\"hidden\" name=\"$name\" value=\"$value\">";
}

////////////////////////////////////////////////////////////////////////

function checkBox($value, $group='', $selected=false, $additional='') {
    static $count = 0;
    if ($group == '') {
        $group = 'checkBox' . ++$count;
    }    $res = '';
    $res .= "<input type=\"checkbox\" name=\"$group\" value=\"$value\"" . ($selected==false ? '' : " checked=\"checked\"") .  ($additional ? " $additional" : '') . ">"; 
    return $res;
}

////////////////////////////////////////////////////////////////////////

function checkBoxes($array, $group='', $selectArray, $additional='') {
    if (is_array($array) && is_array($selectArray)) {
        static $count = 0;
        if ($group == '') {
            $group = 'boxGroup' . ++$count;
        }
        $res = array();
        $i = 0;
        foreach($array as $value) {
            $i++;
            $res[$i]= Forms::checkBox($value, $group, $selectArray[$i - 1], $additional) . "\n";
        }
    }    
    return $res;
}

////////////////////////////////////////////////////////////////////////

function radioButtons($array, $group='', $selectId=1, $additional = '') {
    static $count = 0;
    if ($group == '') {
        $group = 'radioGroup' . ++$count;
    }
    if (is_array($array)) {
        $res = array();
        $i = 0;
        foreach($array as $value) {
            $res[$i++]= Forms::radioButton($value, $group, (int)($selectId == $i), $additional) . "\n";
        }
    }    
    return $res;
}

////////////////////////////////////////////////////////////////////////

function radioButton($value, $group='', $selected=false, $additional='') {
    static $count = 0;
        if ($group == '') {
            $group = 'radioButton' . ++$count;
        }
    $res = '';
    $res .= "<input type=\"radio\" name=\"$group\" value=\"$value\"" . ($selected==false ? '' : " checked=\"checked\"") . ($additional ? " $additional" : '') . ">"; 
    return $res;
}

////////////////////////////////////////////////////////////////////////

function selectBox($array, $selected=0, $name='', $size=1, $multiple=false, $additional='') {
    $res = '';
    static $count = 0;
    if (is_array($array)) {
        if ($name == '') {
            $name = 'selectBox' . ++$count;
        }
        $res .= "<select name=\"$name\" size=\"$size\"" . ($multiple==false ? '' : " multiple=\"multiple\"") . ($additional ? " $additional" : '') . ">\n";
        $i = 0;
        foreach($array as $value) {
            $res .= "<option" . ($selected == ++$i ? " selected=\"selected\"" : '') . ">$value</option>\n";
        }
        $res .="</select>\n";
    }
    return $res;
}

////////////////////////////////////////////////////////////////////////

function submitButton($title = '', $new = false, $newContent = '', $additional='') {
    $res = '';
    if ($new == true) {
        $res .= "<button type=\"submit\" $additional>$newContent$title</button>";
    } else {
        $res .= "<input type=\"submit\" " . ($title == '' ? '' : "value=\"$title\"") . ($additional ? " $additional" : '') . ">";
    }
    return $res;
}

////////////////////////////////////////////////////////////////////////

function resetButton($title = '', $new = false, $newContent = '', $additional='') {
    $res = '';
    if ($new == true) {
        $res .= "<button type=\"reset\" $additional>$newContent$title</button>";
    } else {
        $res .= "<input type=\"reset\" " . ($title == '' ? '' : "value=\"$title\"") . ($additional ? " $additional" : '') . ">";
    }
    return $res;
}

////////////////////////////////////////////////////////////////////////

function button($title, $name = '', $new = false, $newContent = '', $additional = '') {
    static $count = 0;
    if ($name == '') {
        $name = 'button' . ++$count;
    }
    $res = '';
    if ($new == true) {
        $res .= "<button type=\"button\" name=\"$name\" $additional>$newContent$title</button>";
    } else {
        $res .= "<input type=\"button\" name=\"$name\" value=\"$title\"".($additional ? " $additional" : '').">";
    }
    return $res;
}

////////////////////////////////////////////////////////////////////////

function textField($value, $name='', $hidden=false, $size =-1, $length =-1, $additional='') {
    $res = '';
    static $count = 0;
    if ($name == '') {
        $name = 'textField' . ++$count;
    }
    $res .= "<input name=\"$name\" type=\"" . ($hidden ? 'password' : 'text') . "\" value=\"$value\"";
    $res .= ($size != -1 ? " size=\"$size\"" : '');
    $res .= ($length != -1 ? " maxlength=\"$length\"" : '');
    $res .=  ($additional ? " $additional" : '') . ">";
    return $res;
}

////////////////////////////////////////////////////////////////////////

function fileField($name = '', $file = '', $fileSize = 1000000, $size = -1, $accept="text/*", $additional='') {
    static $count = 0;
    if ($name == '') {
        $name = 'fileField' . ++$count;
    }    $res = '';
    $res .= "<input type=\"file\" name=\"$name\"";
    $res .= ($size != -1 ? " size=\"$size\"" : '');
    $res .= ($file != '' ? " value=\"$file\"" : '');
    $res .= " maxlength=\"$fileSize\" accept=\"$accept\"".($additional ? " $additional" : '').">";
    return $res;
}
    

////////////////////////////////////////////////////////////////////////

function textArea($value, $name='', $cols=-1, $rows=-1, $wrap='soft' ,$readOnly=false, $additional='') {
    $res = '';
        static $count = 0;
    if ($name == '') {
        $name = 'textArea' . ++$count;
    }
    $res .= "<textarea name=\"$name\" wrap=\"$wrap\"";
    $res .= ($rows != -1 ? "rows=\"$rows\"" : '');
    $res .= ($cols != -1 ? "cols=\"$cols\"" : '');
    $res .= ($readOnly ? " readonly=\"readonly\"" : '');
    $res .= ($additional ? " $additional" : '') . ">";
    $res .= $value;
    $res .= '</textarea>';
    return $res;
}
////////////////////////////////////////////////////////////////////////
}
?>