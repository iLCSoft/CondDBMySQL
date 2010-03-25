<?php

/*pre: require $novaDB*/
if (!$novaDB->getConnected()) { //connect
  $novaDB->connect();
}

//print browser menu
require_once(PHP_LIB_PATH. "browser_menu.php");
flush();

if (!$novaDB->getConnected()) { //connection with error
  echo("<br/><div class=\"info\">\n");
  echo("Unable to connect to NOVA Database");
  echo("</div> <br/>\n");
}
else {
  require_once(PHP_LIB_PATH . "nova_utils.php");

  echo("<br/><div class=\"info\">\n");
  $no_problem= nova_print_description($_SESSION['nova_id'], $novaDB);
  echo("</div>\n");

  if ($no_problem) {
    //print number of rows found				
    echo("<br/>\n <div class=\"info\">\n");
    echo("Found " . $novaDB->getNumRows() . " row(s).");
    echo("\n</div><br/>\n");


    //get navigations string
   require_once(PHP_LIB_PATH . "utils.php");
    $url="main_frame.php?nova=true&novaDB=" . $novaDB->getDBName() . "&ID=" . $novaDB->getID();
    $index= print_index_footer($url,$_SESSION['from'], $_SESSION['to'], $novaDB->getNumRows()); 

    //print navigation
    if (!empty($index)) {
      echo("\n<div class=\"info\">\n");
      echo($index);
      echo("\n</div>");
      echo("\n<br/>");
    }
    
    echo("<div class=\"info\">");
    $no_problem= nova_print_table($novaDB);
    echo("</div>\n <br/>\n");
    

    //print navigation
    if ($no_problem && !empty($index) ) {
      echo("\n<div class=\"info\">\n");
      echo($index);
      echo("\n</div>");
      echo("\n<br/>");
    }
  }
}


?>