<?php

/**
 * @brief JEWE plugin that handles Chapter keywords
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Config;
use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Plugins;
use JEWE\plugins\Chapters\Chapter;

/**
 * @brief plugin class that handles chapters1
 */
class Chapters implements Plugin, Interpreter {
	const string COMMAND_CHAPTER = 'chapter';
	const string COMMAND_TITLE = 'title';
	const string COMMAND_TASK = 'task';
	const string COMMAND_CHAPTER_CS = 'kapitola';
	const string COMMAND_TITLE_CS = 'nadpis';
	const string COMMAND_TASK_CS = 'úkol';
	const string COMMAND_TASK_CS_ALT = 'ukol';

	/**
	 * Reference to chapter root
	 */
	private(set) Chapter $root;

	/**
	 * Reference to the current chapter
	 */
	private Chapter $chapter;

	/**
	 * Paths for execute() method
	 */
	private array $paths;


	/**
	 * @brief  returns version of this plugin
	 * @return [float] version
	 */
	public static function getVersion() : float
	{
		return 1.0;
	} //getVersion()


	/**
	 * @brief  returns an array of keywords which the plugin is able to handle
	 * @return string[]
	 */
	public static function getCommands() : array
	{
		return [
			self::COMMAND_CHAPTER,
			self::COMMAND_TASK,
			self::COMMAND_TITLE,
			self::COMMAND_CHAPTER_CS,
			self::COMMAND_TASK_CS,
			self::COMMAND_TASK_CS_ALT,
			self::COMMAND_TITLE_CS,
		];
	} //getCommands()


	public function init(int $kind) : bool
	{
		//create a root chapter
		$this->root = new Chapter();
		$this->chapter = $this->root; //the initial chapter is root
		$this->paths = [];
		return true;
	} //init()


	public function execute(string $command, string $params) : ?string
	{
		$skip = Config::get('skipFirstChapter');

		switch ($command) {
			case self::COMMAND_CHAPTER:
			case self::COMMAND_CHAPTER_CS:
				$index = $this->addChapter(0, $params);
				if (!$skip) {
					++$index;
				}
				$this->paths[0] = (string)$index;
				return '<a class="totop" href="#topmenu">Hlavní menu</a><hr>'
				       . '<a name="mainmenu_' . $this->paths[0] . '"><h2>' . (0 === $index ? '' : $index . '. ') . $params . '</h2></a>';
				break;
			case self::COMMAND_TITLE:
			case self::COMMAND_TITLE_CS:
				$index = $this->addChapter(1, $params);
				++$index;
				$this->paths[1] = $this->paths[0] . '_' . $index;
				return '<br><a class="totop" href="#topmenu">Hlavní menu</a>'
				       . '<a name="mainmenu_' . $this->paths[1] . '"><h3>' . $index . '. ' . $params . '</h3></a>';
				break;
			case self::COMMAND_TASK:
			case self::COMMAND_TASK_CS:
			case self::COMMAND_TASK_CS_ALT:
				$index = $this->addChapter(2, $params);
				++$index;
				$this->paths[2] = $this->paths[1] . '_' . $index;
				return '<br><a class="totop" href="#topmenu">Hlavní menu</a>'
				       . '<a name="mainmenu_' . $this->paths[2] . '"><h4>' . $index . '. ' . $params . '</h4></a>';
				break;
		} // switch

		return null;
	} //execute()


	/**
	 * @brief  Sets the current level.
	 * @param  int The requested level
	 * @param  null reserved
	 * @return int|bool|null Integer for success (number of steps), false for error, null for root
	 */
	public function gotoLevel($level, $step = 0) : int|bool|null
	{
		$currentLevel = $this->chapter->getLevel();
		if ($level === $currentLevel) {
			//end of recurse
			return $step;
		}

		if ($level < $currentLevel) {
			//go to the previous (parent) level
			if ($this->chapter->isRoot()) {
				//we are already in root - an error?
				return null;
			}

			$this->chapter = $this->chapter->getParent();
			return $this->gotoLevel($level, --$step); //chapter is root
		} //end of recurse

		//go to the previous level

		//go to the next level
		$chapter = $this->chapter->getChapter(); //get active subchapter
		if (null === $chapter) {
			//an active chapter does not exist
			return false;
		}

		//go to the next level
		$this->chapter = $chapter;
		return $this->gotoLevel($level, ++$step); //chapter exists
	} //gotoLevel()


	/**
	 * Adds a new chapter to the requested level
	 * @param  int $level level for the chapter
	 * @param  string $caption caption of the chapter
	 * @return int index of the new chapter in its level
	 */
	public function addChapter(int $level, string $caption) : int
	{
		if (false === $this->gotoLevel($level)) {
			echo("<script>alert('Cannot create chapter $caption in level $level!');</script>");
		}
		$index = $this->chapter->addChapter($caption);
		//set the active chapter to the last one
		$this->chapter = $this->chapter->getChapter();
		return $index;
	}


	public function getRoot() : Chapter
	{
		return $this->root;
	}
} //class Chapters

//Register this class as Interpreter and Observer
Plugins::register(Plugins::INTERPRETER, 'Chapters');
