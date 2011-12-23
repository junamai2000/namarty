#!/usr/bin/php
<?php
dl('namarty.so');
include("Smarty/libs/Smarty.class.php");

$case1 = array(
	array(
		'first'=>'case1_FIRST1',
		'second'=>'case1_SECOND2',
	),
	array(
		'first'=>'case1_FIRST2',
		'second'=>'case1_SECOND2',
	),
	array(
		'first'=>'case1_FIRST3',
		'second'=>'case1_SECOND3',
	),
);
$case2 = array(
	array(
		'first'=>'case2_FIRST1',
		'second'=>'case2_SECOND2',
	),
	array(
		'first'=>'case2_FIRST2',
		'second'=>'case2_SECOND2',
	),
	array(
		'first'=>'case2_FIRST3',
		'second'=>'case2_SECOND3',
	),
);
$case = array();
$case = array();
$first = "1";
$last = "junya";
$type = "n";

$time_start = microtime(true);
$namarty = new namarty();
$namarty->assign("FirstName", $first);
$namarty->assign("LastName", $last);
$namarty->assign("case1", $case1);
$namarty->assign("case2", $case2);
$namarty->display("/home/junya/namarty/tests/template/foreach.tpl");
$time_end = microtime(true);
$time = $time_end - $time_start;
echo "namarty takes ". $time ." sec.\n";

?>
