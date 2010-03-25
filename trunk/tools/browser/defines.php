<?php

/*This file declares the environment constants*/

/*root path of the browser*/
define("ROOT_PATH", getcwd() . "/");
/*root URL of the browser*/
define("ROOT_WWW_PATH",substr($url, 0, strrpos($url, "/")+1 ));

/*lib path*/
define("LIB_PATH", ROOT_PATH . "lib/");
define("LIB_WWW_PATH", ROOT_WWW_PATH . "lib/");

/*Javascript library path*/
define("JS_LIB_PATH", LIB_PATH . "js/");
define("JS_LIB_WWW_PATH", LIB_WWW_PATH . "js/");

/*PHP library path*/
define("PHP_LIB_PATH", LIB_PATH . "php/");
define("PHP_LIB_WWW_PATH", LIB_WWW_PATH . "php/");

/*PHP Classes library path*/
define("CLASS_PATH", PHP_LIB_PATH . "class/");
define("CLASS_WWW_PATH", PHP_WWW_LIB_PATH . "class/");

/*3rd Party code path*/
define("THIRD_PARTY_LIB_PATH", LIB_PATH . "3rd_party/");
define("THIRD_PARTY_WWW_LIB_PATH", LIB_WWW_PATH . "3rd_party/");

/*ccs path*/
define("CSS_PATH", ROOT_PATH  . "css/");
define("CSS_WWW_PATH", ROOT_WWW_PATH . "css/");

/*constats for the browser menu*/
define('B_POINT', 1);
define('B_HISTORY', 2);
define('B_TAG', 3);

/*constants for folder type defenition*/
define('BLOBTAG', 0);
define('STRUCT', 1);
define('STRUCTID', 2);
define('BLOB', 3);
define('STRUCTTAG', 4);
define('STRUCTIDTAG', 5);

/*constats for column type defenition of condTables*/
define('CDBNULL', 0);
define('CDBBOOL', 1);
define('CDBINT', 2);
define('CDBFLOAT', 3);
define('CDBSTRING', 4);
define('CDBDOUBLE', 5);
define('CDBLONGLONG', 6);
define('CDBARRAYBOOL', 7);
define('CDBARRAYFLOAT', 8);
define('CDBARRAYINT', 9);
define('CDBARRAYSTRING', 10);
define('CDBARRAYDOUBLE', 11);
define('CDBARRAYLONGLONG', 12);

define('MAX_INTERVAL', 100);
define('INDEX_LIMIT', 10);

/*constants for the environment defenition*/
define('S_START', 0); //starting browser
define('S_FOLDER', 1); //browsing folder
define('S_FOLDER_SET', 2); //browsing folder set
define('S_BLOB_TAG', 3); //browsing blob with tag
define('S_BLOB_POINT', 4); //browsing blob at point
define('S_BLOB_HISTORY', 5); //browsing blob history
define('S_BLOB_DETAIL', 6); //browsing blob detail
define('S_TABLE_TAG', 7); //browsing table with tag
define('S_TABLE_POINT', 8); //browsing table at point
define('S_TABLE_HISTORY', 9); //browsing table history
define('S_NOVA', 10); //browsing NOVA
define('S_UNKNOWN', 99); //unknow state

/*version*/
define('VERSION', "1.1c");

/*database connection data*/
if ($_SESSION['changedDB']) {
  define("DB_HOST", $_SESSION['host']);
  define("DB_NAME", $_SESSION['database']);
  define("DB_USER", $_SESSION['username']);
  define("DB_PASS", $_SESSION['password']);
}
else {
	define("DB_HOST", "kdata8");
	//    define("DB_NAME", "test_conditionsExamples");
	   define("DB_NAME", "test_tiago");
	   define("DB_USER", "tfranco");
   define("DB_PASS", "pa\$\$L123");
}


?>
