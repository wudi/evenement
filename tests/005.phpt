--TEST--
Check for EventEmitterTrait demo
--SKIPIF--
<?php if (!extension_loaded("evenement")) print "skip"; ?>
--FILE--
<?php
class event {
    use \Evenement\EventEmitterTrait;

}

$event = new event();
$event->on('key1', 'foo');
--EXPECT--
PHP Catchable fatal error:  Argument 2 passed to event::on() must be callable, string given in /Users/eagle/SourceCode/php-src/ext/evenement/t.php on line 8

Catchable fatal error: Argument 2 passed to event::on() must be callable, string given in /Users/eagle/SourceCode/php-src/ext/evenement/t.php on line 8

