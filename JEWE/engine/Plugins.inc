<?php

/**
 * @brief class for plugin registration
 * @version JEWE 3.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\engine;

use Exception;
use UnexpectedValueException;
use InvalidArgumentException;
use RuntimeException;

/**
 * @singleton
 * @brief  plugin registration class
 */
final class Plugins extends Singleton {
	private const string FILE_MASK_REG_EXP = '/\.(inc|php)$/i';
	private const string FOLDER = __DIR__ . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'plugins' . DIRECTORY_SEPARATOR;
	private const string NAMESPACE_PREFIX = 'JEWE\\plugins\\';


	/** @var array $plugins List of all registered plugins */
	public private(set) array $plugins;

	/** @var array $commands List of all commands and what executest them */
	private array $commands;

	/** @var array $events List of all events and its observers */
	private array $events;

	public const int OTHER = 0;
	public const int INTERPRETER = 1;
	public const int EXPLORER = 2;
	public const int OBSERVER = 3;


	/**
	 * Looks for plugin files and lets them register
	 */
	public static function init() : void
	{
		//find all plugin files
		$files = scandir(self::FOLDER);

		//let the plugins register
		foreach ($files as $plugin) {
			if (is_dir(self::FOLDER . $plugin)) {
				continue; //skip folders
			}
			if (!preg_match(self::FILE_MASK_REG_EXP, $plugin)) {
				continue; //not a PHP file
			}
			include_once(self::FOLDER . $plugin);
		}

		foreach (self::getSelf()->plugins as $kind => $plugins) {
			foreach ($plugins as $plugin) {
//				echo 'Initializing plugin ' . $plugin['name'] . ' of kind ' . $kind, PHP_EOL;
				$success = $plugin['instance']->init($kind);

				if (!$success) {
					self::unregister($plugin['kind'], $plugin['name']);
				}
			}
		}
	} //init()


	/**
	 * @brief  registers new plugin - calls private method over the instance
	 * @param  int $kind type of the plugin
	 * @param  string $name class name of the plugin
	 */
	public static function register(int $kind, string $name) : bool
	{
		//check if the plugin was not already registered
		if (self::isPluginRegistered($kind, $name)) {
			return false;
		}
		//register plugin
		try {
			self::getSelf()->registerPlugin($kind, $name);
			return true;
		}
		catch (Exception $e) {
			return false;
		}
	} //register()


	public static function unregister($kind, $name) : bool
	{
		//check if the plugin is registered
		if (!self::isPluginRegistered($kind, $name)) {
			return false;
		}
		//register plugin
		try {
			self::getSelf()->unregisterPlugin($kind, $name);
			return true;
		}
		catch (Exception $e) {
			return false;
		}
	} //unregister


	/**
	 * @brief  registers new plugin
	 * @param  int $kind Type of the plugin
	 * @param  string $name Class name of the plugin
	 * @throws UnexpectedValueException When a plugin does not return expected data
	 * @thours InvalidArgumentException When an unknown plugin kind is given
	 */
	private function registerPlugin(int $kind, string $name) : void
	{
		$name = self::NAMESPACE_PREFIX . $name;
		$plugin = new $name();

		if (!isset($this->plugins[$kind])) {
			$this->plugins[$kind] = [];
		}

		$this->plugins[$kind][$name] = [
			'name' => $name,
			'kind' => $kind,
			'instance' => &$plugin,
			'version' => $plugin->getVersion(),
		];

		switch ($kind) {
			case self::INTERPRETER:
				$commands = $plugin->getCommands();
				if (!is_array($commands)) {
					throw new UnexpectedValueException('Plugin ' . $name . ' does not return any commands!');
				}
				foreach ($commands as $command) {
					if (!isset($this->commands[$command])) {
						$this->commands[$command] = [
							'interpreter' => $name,
							'instance' => &$plugin,
						]; //store new command
					} //if new command
				} //foreach command
				$this->plugins[$kind][$name]['commands'] = $commands;
				break;
			case self::OBSERVER:
				$events = $plugin->getEvents();
				if (!is_array($events)) {
					throw new UnexpectedValueException('Plugin ' . $name . ' does not return any events!');
				}
				foreach ($events as $event) {
					if (!isset($this->events[$event])) {
						$this->events[$event] = [];
					}
					$this->events[$event][$name] = [
						'observer' => $name,
						'instance' => $plugin,
					];
				} //foreach event
				$this->plugins[$kind][$name]['events'] = $events;
				break;
			default:
				throw new InvalidArgumentException('Unknown plugin kind ' . $kind);
		} // switch
	}


	/**
	 * @brief  removes plugin from registration
	 * @param  int $kind type of the plugin
	 * @param  string $name class name of the plugin
	 * @throws InvalidArgumentException When an unknown plugin kind is given
	 */
	private function unregisterPlugin($kind, $name) : void
	{
		switch ($kind) {
			case self::INTERPRETER:
				//remove plugin from commands
				foreach ($this->plugins[$kind][$name]['commands'] as $command) {
					unset($this->commands[$command]);
				}
				break;
			case self::OBSERVER:
				//remove plugin from events
				foreach ($this->plugins[$kind][$name]['events'] as $event) {
					unset($this->events[$event][$name]);
				}
				break;
			default:
				throw new InvalidArgumentException('Uknown plugin kind ' . $kind . '');
		} // switch

		unset($this->plugins[$kind][$name]);
	}


	/**
	 * @brief  returns true if the plugin was already registered
	 * @param  int $kind type of the plugin
	 * @param  string $name name of the plugin
	 */
	public static function isPluginRegistered(int $kind, string $name) : bool
	{
		return isset(self::getSelf()->plugins[$kind][$name]);
	} //isPluginRegistered()


	/**
	 * @brief  returns reference to the instance of the plugin
	 * @param  int $kind type of the plugin
	 * @param  string $name name of the plugin
	 */
	public static function getPlugin(int $kind, string $name) : ?Plugin
	{
		if (self::isPluginRegistered($kind, $name)) {
			return self::getSelf()->plugins[$kind][$name]['instance'];
		}
echo 'Plugin ' . $name . ' of kind ' . $kind . ' is not registered in ['.implode(', ', array_keys(self::getSelf()->plugins[$kind])) . ']', PHP_EOL;
		return null;
	} //getPlugin()


	public static function getSelf() : Plugins
	{
		return Singleton::getInstance(__CLASS__);
	}


	/**
	 * @brief  raises event and notifies all observers
	 * @param  int $event Event to rise
	 * @param  array|null $params Params for the event
	 * @return string[] results from all observers
	 */
	public function riseEvent(int $event, ?array $params) : array
	{
		$result = [];
		foreach ($this->events[$event] as $plugin) {
			$result[$plugin['observer']] = $plugin['instance']->observe($event, $params);
		}
		return $result;
	} //riseEvent()


	/**
	 * @brief  executes command using registered plugin
	 */
	public static function executeCommand(string $command, string $params) : bool|string
	{
		$instance = self::getSelf();

		if (isset($instance->commands[$command])) {
			return $instance->commands[$command]['instance']->execute($command, $params);
		}

		return false;
	}

} //class Plugins
