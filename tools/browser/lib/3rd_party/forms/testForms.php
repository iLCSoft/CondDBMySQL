<html>
<body>
<?php
////////////////////////////////////////////////////////////////////////
/*
    example for Forms.php
*/
////////////////////////////////////////////////////////////////////////

require "Forms.php";

////////////////////////////////////////////////////////////////////////

echo Forms::openForm(basename(__FILE__));

$array = array('Entry 1', 'Entry 2', 'Entry 3', 'Entry 4');

echo Forms::selectBox($array, 3) . '<br>';

echo Forms::openFieldSet('set 1');
$i = 0;
foreach(Forms::radioButtons($array) as $value) {
    echo $value . $array[$i++];
}
echo Forms::closeFieldSet();

echo Forms::openFieldSet('set 2');
$i = 0;
foreach(Forms::checkBoxes($array, '', array(true, false, true, true)) as $value) {
    echo $value . $array[$i++] . "<br>";   
}
echo Forms::closeFieldSet();

echo Forms::textField('Type here') . "<br>";
echo Forms::textArea('Type here','', 20, 5) . "<br>";

echo Forms::resetButton();
echo Forms::submitButton();

echo Forms::closeForm();

////////////////////////////////////////////////////////////////////////
?>
</body>
</html>
