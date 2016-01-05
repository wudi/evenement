# Evenement

Evenement is a very simple event dispatching library for PHP write in C. Base on [@igorw/evenement](https://github.com/igorw/evenement) 


## Build extension

```
 git clone git@github.com:putil/evenement.git
 cd evenement
 /usr/local/bin/phpize
 ./configure --with-php-config=/usr/local/bin/php-config
 make && make install && make clean
 echo "extension=evenement.so" >> /usr/local/etc/php/5.5/php.ini
```


## Usage

### Creating an Emitter

```php
<?php
$emitter = new Evenement\EventEmitter();
```

### Adding Listeners

```php
<?php
$emitter->on('user.created', function (User $user) use ($logger) {
    $logger->log(sprintf("User '%s' was created.", $user->getLogin()));
});
```

### Emitting Events

```php
<?php
$emitter->emit('user.created', array($user));
```

