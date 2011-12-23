#!/usr/bin/php
<?php
dl('namarty.so');
dl('simplate.so');
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
$first = "苗字生井";
$last = "junya";
$type = "n";
if ($type == "n") {
	$time_start = microtime(true);
	$namarty = new Namarty();
	$namarty->assign("FirstName", $first);
	$namarty->assign("LastName", $last);
	$namarty->assign("case1", $case1);
	$namarty->assign("case2", $case2);
	$namarty->display("/home/junya/DL/php-5.2.6/ext/namarty/tests/template/namarty_test.tpl");
	$time_end = microtime(true);
	$time = $time_end - $time_start;
	echo "namarty ". $time ."秒掛かりました。\n";
} else if ($type == "s") {
	$time_start = microtime(true);
	$smarty = new Smarty();
	$smarty->assign("FirstName", $first);
	$smarty->assign("LastName", $last);
	$smarty->assign("case1", $case1);
	$smarty->assign("case2", $case2);
	$smarty->display("/home/junya/DL/php-5.2.6/ext/namarty/tests/template/namarty_test.tpl");
	$time_end = microtime(true);
	$time = $time_end - $time_start;
	echo "smarty ". $time ."秒掛かりました。\n";
} else if ($type == "m") {
	$time_start = microtime(true);
	$simplate = new Simplate();
	$simplate->template_dir = 'template'; 
	$simplate->compile_dir = 'template_c'; 
	$simplate->left_delimiter = '{';
	$simplate->right_delimiter = '}';
	$simplate->assign("FirstName", $first);
	$simplate->assign("LastName", $last);
	$simplate->assign("case1", $case1);
	$simplate->assign("case2", $case2);
	$simplate->display("namarty_test.tpl");
	$time_end = microtime(true);
	$time = $time_end - $time_start;
	echo "simplate ". $time ."秒掛かりました。\n";
}
?>
