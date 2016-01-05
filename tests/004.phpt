--TEST--
Check for EventEmitterTrait exists
--SKIPIF--
<?php if (!extension_loaded("evenement")) print "skip"; ?>
--FILE--
<?php 
var_dump(trait_exists("Evenement\EventEmitterTrait"));
?>
--EXPECT--
bool(true)
