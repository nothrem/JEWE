<?php
declare(strict_types=1);

namespace JEWE\helpers;

use Generator;

trait GetFiles {
	protected static function getFiles(string $class, string $dir) : Generator {
		$path = explode('\\', $class); //Separator in class namespaces (backslashes), not a directory separator!
		$folder = array_pop($path);

		$dir = rtrim($dir, DIRECTORY_SEPARATOR);

		$files = scandir($dir . DIRECTORY_SEPARATOR . $folder);

		foreach ($files as $file) {
			if (is_dir(__DIR__ . DIRECTORY_SEPARATOR . $folder . DIRECTORY_SEPARATOR . $file)) {
				continue;
			}
			if (str_starts_with($file, '.')) {
				continue;
			}
			yield $folder . DIRECTORY_SEPARATOR . $file;
		}
	}
}
