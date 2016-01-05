--TEST--
Check for EventEmitterInterface
--SKIPIF--
<?php if (!extension_loaded("evenement")) print "skip"; ?>
--FILE--
<?php
interface_exists("Evenement\EventEmitterInterface");

class event implements Evenement\EventEmitterInterface
{
    private $event = array();

    public function on($event, callable $listener)
    {
        $this->event[(string)$event] = $listener;
        return true;
    }

    public function once($event, callable $listener)
    {

    }

    public function removeListener($event, callable $listener)
    {

    }

    public function removeAllListeners($event = null)
    {

    }

    public function listeners($event)
    {

    }

    public function emit($event, array $arguments = array())
    {
        if(isset($this->event[$event])){
            return call_user_func_array($this->event[$event], $arguments);
        }

        return false;
    }
}

$e = new event();
$ret = $e->on('start', function(){
    return "call start event";
});
var_dump($ret);

$ret = $e->emit('start');
var_dump($ret);
?>
--EXPECT--
bool(true)
string(16) "call start event"
