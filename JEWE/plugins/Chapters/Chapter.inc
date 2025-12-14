<?php

/**
 * @brief Chapter for Chapters plugin
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins\Chapters;

use Iterator;

/**
 * Class for one chapter in Chapters plugin
 *
 */
class Chapter implements Iterator {

	private(set) int $level;
	private(set) string $caption;
	private(set) ?Chapter $parent = null;

	private(set) array $chapters;
	private int $position = 0;


	/**
	 * @constructor
	 */
	public function __construct(?Chapter $parent = null, ?string $caption = null){
		if (isset($parent)) {
			$this->level = $parent->level + 1;
			$this->caption = $caption;
			$this->parent = $parent;
		}
		else {
			//this chapter is root
			$this->level = 0;
			$this->caption = 'Hlavní menu';
			$this->parent = null;
		}
		$this->chapters = array();
		$this->position = 0;
	}

	public function isEmpty() : bool
	{
		return empty($this->chapters);
	}

	/**
	 * @deprecated use property $level instead
	 */
	public function getLevel() : int
	{
		return $this->level;
	}

	public function isRoot() : bool
	{
		return (null === $this->parent) && (0 === $this->level);
	}

	/**
	 * @deprecated use property $parent instead
	 */
	public function getParent() : Chapter {
		return $this->parent;
	}

	/**
	 * Sets a new caption for this chapter
	 */
	public function setCaption(string $caption) : void
	{
		$this->caption = $caption;
	}

	public function getChapter(?int $chapter = null) {
		if (null === $chapter) {
			$chapter = count($this->chapters) - 1;
		}

		return $this->chapters[$chapter] ?? null;
	}

	public function addChapter(?string $caption = null) : int
	{
		$this->chapters[] = new Chapter($this, $caption);
		return count($this->chapters) - 1;
	}

	/**
	 * methods for array iteration
	 */
	public function rewind() : void
	{
		$this->position = 0;
	}


	public function current() : mixed
	{
		return $this->chapters[$this->position];
	}


	public function key() : int
	{
		return $this->position;
	}


	public function next() : void
	{
		++$this->position;
	}


	public function valid() : bool
	{
		return isset($this->chapters[$this->position]);
	}


} //class Chapter
