#!/usr/bin/php
<?php
dl('namarty.so');
$first = "1";
$last = "junya";
$type = "n";

$time_start = microtime(true);
$namarty = new Namarty();
$namarty->assign("FirstName", $first);
$namarty->assign("LastName", $last);
$namarty->display("./template/if.tpl");
$time_end = microtime(true);
$time = $time_end - $time_start;
echo "namarty ". $time ."秒掛かりました。\n";
?>
