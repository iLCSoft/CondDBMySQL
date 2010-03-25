<?php
//close current session to show welcome message
if (isset($_POST['restart'])) {
  session_start();
  session_unset();
  session_destroy();
}
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN"
"http://www.w3.org/TR/REC-html40/frameset.dtd">
<html lang="en">

<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="autor" content="Gama Franco">
<meta name="description" content="Atlas-DAQ">
<meta name="keywords" content="Atlas, DAQ, CERN, ConditionsDB, Conditions, Database">
<title>Conditions Database Web Browser</title>

<frameset cols="250,*" >
  <frame src="main_menu.php" name="mainMenu" scrolling="yes" noresize frameborder="0">
  <frame src="main_frame.php" name="mainFrame" scrolling="yes" noresize frameborder="0">

<!-- some browsers do not support frames //-->
<noframes>
<p>
Your browser doesn't support frames, please upgrade.
</p>
</noframes>

</frameset>


</html>
