<?php
declare(strict_types=1);


namespace JEWE;

/**
 * PSR-4 compatible class autoloader for JEWE namespace
 */


spl_autoload_register(static function (string $class) : void {
//	echo 'Loading class: ', $class, PHP_EOL;

	$baseDir = __DIR__;
//	echo 'JEWE is installed in folder: ', $baseDir, PHP_EOL;

	$prefix = __NAMESPACE__ . '\\'; //Namespace for all classes in this framework
	$prefixLength = strlen($prefix);

	if (strncmp($prefix, $class, $prefixLength) !== 0) {
//		echo 'Skipping class from another framework: ', $class, PHP_EOL;
		return;
	}

	$relativeClass = substr($class, $prefixLength);
	$file = $baseDir . DIRECTORY_SEPARATOR .
			str_replace('\\', DIRECTORY_SEPARATOR, $relativeClass);

//	echo 'Looking for file ', $file, '.inc...', PHP_EOL;

	if (file_exists($file . '.php')) {
		require_once $file . '.php';
	}
	elseif (file_exists($file . '.inc')) {
		require_once $file . '.inc';
	}
	else {
		throw new \RuntimeException(sprintf('**Class "%s" not found in %s.php or %s.inc.', $class, $file, $file));
	}
});
