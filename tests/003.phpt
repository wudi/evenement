--TEST--
Check for EventEmitter exists
--SKIPIF--
<?php if (!extension_loaded("evenement")) print "skip"; ?>
--FILE--
<?php 
var_dump(class_exists("Evenement\EventEmitter"));
?>
--EXPECT--
bool(true)
