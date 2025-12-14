<?php
declare(strict_types=1);

namespace JEWE\plugins;

/**
 * @brief JEWE plugin that handles keywords for game Mass Effect
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

use JEWE\engine\Config;
use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Plugins;
use JEWE\helpers\GetFiles;
use InvalidArgumentException;
use RuntimeException;

/**
 * @brief plugin class that handles keywords for game Mass Effect
 */
class MassEffect implements Plugin, Interpreter {
	use GetFiles;

	private HtmlParser $parser;
	private $includeIndex;


	public static function getVersion() : float
	{
		return 1.0;
	} //getVersion()


	public static function getCommands() : array
	{
		return [
			'ME',
			'ME_space',
		];
	} //getCommands()


	public function init(int $kind) : bool
	{
		$plugin = Plugins::getPlugin(Plugins::OBSERVER, HtmlParser::class);
		if (!$plugin instanceof HtmlParser) {
			$plugins = Plugins::getSelf()->plugins[Plugins::OBSERVER];
			throw new RuntimeException('Could not find HtmlParser plugin in ['.implode(', ', array_keys($plugins)) . ']');
		}
		$this->parser = $plugin;
		$this->includeIndex = 0;
		return true;
	} //init()


	public function execute(string $command, string $params) : ?string
	{
		switch ($command) {
			case 'ME':
				$values = explode(' ', $params, 2);
				switch ($values[0]) {
					case 'bug':
						return '<p class="ME bug">' . $values[1] . '</p>';
						break;
					case 'postava':
						return '<p class="ME postava">Vytvoření postavy: ' . $values[1] . '</p>';
						break;
					case 'include':
						$fileName = trim($values[1]);
						$fileName = getcwd() . '/' . $fileName;

						if (is_file($fileName)) {
							$file = file($fileName);
							$this->parser->process($file, [
								'hvězda' => 'ME_space',
								'hvezda' => 'ME_space',
								'system' => 'ME_space',
								'planeta' => 'ME_space',
								'místo' => 'ME_space',
								'misto' => 'ME_space',
								'měsíc' => 'ME_space',
								'mesic' => 'ME_space',
								'úkol' => 'ME_space',
								'ukol' => 'ME_space',
								'nález' => 'ME_space',
								'nalez' => 'ME_space',
								'prvky' => 'ME_space',
								'plyny' => 'ME_space',
								'kovy' => 'ME_space',
								'medailon' => 'ME_space',
								'insignie' => 'ME_space',
								'spisy' => 'ME_space',
								'disk' => 'ME_space',
								'id' => 'ME_space',
								'šavlozubec' => 'ME_space',
							]);
							$this->includeIndex++;
							return '<p id="ME_space_' . $this->includeIndex . '_on" class="ME space">'
							       . '<a href="open://space_system_' . $this->includeIndex . '" onclick=\''
							       . 'getElementById("ME_space_' . $this->includeIndex . '_off").style.display = "";'
							       . 'getElementById("ME_space_' . $this->includeIndex . '_on").style.display = "none";'
							       . 'return false;\'>Zobrazit hvězdokupu</a></p>'

							       . '<div id="ME_space_' . $this->includeIndex . '_off" class="ME space" style="display:none">'
							       . '<p class="ME space">'
							       . '<a href="close://space_system_' . $this->includeIndex . '" onclick=\''
							       . 'getElementById("ME_space_' . $this->includeIndex . '_on").style.display = "";'
							       . 'getElementById("ME_space_' . $this->includeIndex . '_off").style.display = "none";'
							       . 'return false;\'>Skrýt hvězdokupu</a></p>'
							       . implode("\n", $file)
							       . '</div>';
						}

						return "<script>alert('Error: File $fileName not found!');</script>\n";
						break;

					default:
						return '<p class="ME" style="font-size:125%">' . $values[0] . ': ' . $values[1] . '</p>';
				} // switch
				break;
			case 'ME_space':
				$values = explode(' ', $params, 2);
				switch ($values[0]) {
					case 'hvězda':
					case 'hvezda':
					case 'system':
						return '<p class="ME system">' . $values[1] . '</p>';
						break;
					case 'planeta':
						$values = explode(' ', $values[1], 2);
						$type = match ($values[0]) {
							'skalnatá' => 'Skalnatá planeta ' . $values[1],
							'kovová' => 'Kovová planeta ' . $values[1],
							'sopečná' => 'Sopečná planeta ' . $values[1],
							'ledová' => 'Ledová planeta ' . $values[1],
							'pouštní' => 'Pouštní planeta ' . $values[1],
							'zelená' => 'Zelená planeta ' . $values[1],
							'vodní' => 'Vodní svět ' . $values[1],
							'obydlená' => 'Obydlená planeta ' . $values[1],
							'obr' => 'Plynný obr ' . $values[1],
							default => 'Planeta ' . (array_key_exists(1, $values) ? $values[1] : $values[0]),
						}; // switch
						return '<p class="ME misto">' . $type . '</p>';
						break;
					case 'místo':
					case 'misto':
						return '<p class="ME misto">' . $values[1] . '</p>';
						break;
					case 'měsíc':
					case 'mesic':
						return '<p class="ME misto sub">' . $values[1] . '</p>';
						break;
					case 'úkol':
					case 'ukol':
						return '<p class="ME ukol">* Úkol: ' . $values[1] . '</p>';
						break;
					case 'nalez':
					case 'nález':
						return '<p class="ME nalez">- Nález: ' . $values[1] . '</p>';
						break;
					case 'prvky':
						return '<p class="ME nalez">- Vzácné prvky: ' . $values[1] . '</p>';
						break;
					case 'plyny':
						return '<p class="ME nalez">- Vzácné plyny: ' . $values[1] . '</p>';
						break;
					case 'kovy':
						$values[1] = explode(' ', $values[1], 2);
						if (!array_key_exists(1, $values[1])) {
							throw new InvalidArgumentException('Invalid params for command "' . $command . '(' . var_export($values, true) . ')"');
						}
						return '<p class="ME nalez">- '
						       . ('těžké' === $values[1][0] ? 'Těžké' : 'Lehké')
						       . ' kovy: ' . $values[1][1] . '</p>';
						break;
					case 'medailon':
						return '<p class="ME nalez">- Salarianský medailon Ligy Jednoho</p>';
						break;
					case 'insignie':
						return '<p class="ME nalez">- Turianská insignie: ' . $values[1] . '</p>';
						break;
					case 'spisy':
						return '<p class="ME nalez">- Asarijské spisy: Zápisky Matriarchy Dilinagy</p>';
						break;
					case 'disk':
						return '<p class="ME nalez">- Protheanský datový disk</p>';
						break;
					case 'id':
						return '<p class="ME nalez">- Salarianské ID: ' . $values[1] . '</p>';
						break;
					case 'šavlozubec':
						return '<p class="ME nalez">+ Šavlozubec</p>';
						break;
					default:
						return 'Neznámý příkaz "' . $values[0] . '" s parametrem "' . $values[1] . '"<br>';
				} // switch
		}
		return ''; //no HTML output
	} //execute()


	public static function getEvents() : array
	{
		return [
			Events::ON_HEADER,
		];
	}


	public function observe($event, $params) : ?string
	{
		if ($event === Events::ON_HEADER) {
			$lines = Styles::getClassStyles(self::class);
			return implode("\n", $lines);
		} // switch

		return null;
	}

} //class Chapters

//Register this class as Interpreter and Observer
Plugins::register(Plugins::INTERPRETER, 'MassEffect');
Plugins::register(Plugins::OBSERVER, 'MassEffect');
