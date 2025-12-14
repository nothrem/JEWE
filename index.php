<?php

use JEWE\engine\Events;
use JEWE\engine\Plugins;

//load configuration
require_once('config.inc');

//Initialize JEWE framework
require_once(__DIR__ . '/JEWE/load.inc');

//initialize all plugins
Plugins::init();

//tell listeners they can start working
$result = Events::start();

echo implode(PHP_EOL, $result);
