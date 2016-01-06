<?php
trait a{
    protected $listeners;

    public function on(){
        $this->listeners = [123];

        return $this->listeners;
    }

}

class b{
    use a;
}

$b = new b;
var_dump($b->on());

class event {
    use \Evenement\EventEmitterTrait;
}

$event = new event();
var_dump($event);
$event->on('key1', function($msg){
    echo "key1 event: \n";
    var_dump($msg);
});
$event->on('key2', function($val){
    return $val+1;
});
class env{
    public static function foo($val){
        return $val+1;
    }
}

$env_foo = "env::foo";
$event->on('key3', function(){
});
$event->on('key3', $env_foo);

$event->on('key4', [new env(), 'foo']);
$event->on('key4', function(){
    return 0;
});

var_dump($event);

echo "removeListener\n\n";

$event->removeListener("key3", $env_foo);

var_dump($event->listeners['key3']);
$ret = array_search( $env_foo, $event->listeners['key3'], true);
var_dump($ret);
echo "\n\n";
var_dump($event);

echo "\nlisteners\n";

$value = $event->listeners('key4');
var_dump($value);

$value = $event->listeners('key_none');
var_dump($value);

echo "\nemit\n";
$event->emit('key1', ["Hello World"]);
$event->emit('key1', [new env()]);

echo "\nremoveAllListeners\n";
$event->removeAllListeners();
var_dump($event);

$emitter = new Evenement\EventEmitter();
$emitter->on('user.created', function ($user) {
    var_dump($user);
});

var_dump($emitter);

$emitter->emit('user.created', array("eagle"));



