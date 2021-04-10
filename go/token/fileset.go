/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * file.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

import "github.com/cosmos72/onejit/go/sort"

// functionally equivalent to go/token.FileSet
type FileSet struct {
	dirname string
	files   []*File
}

func NewFileSet(dirname string) *FileSet {
	return &FileSet{dirname: dirname}
}

func (f *FileSet) Name() string {
	if f == nil {
		return ""
	}
	return f.dirname
}

func (f *FileSet) Size() int {
	n := len(f.files)
	if n == 0 {
		return 0
	}
	last := f.files[n-1]
	if end := last.End(); end >= 0 {
		return end
	}
	n = len(last.line)
	return last.line[n-1]
}

func (f *FileSet) AddFile(name string) *File {
	file := NewFile(name, f.Size())
	f.files = append(f.files, file)
	return file
}

// return the Position for the given Pos p
func (f *FileSet) Position(p Pos) Position {
	if f == nil || len(f.files) == 0 || p == NoPos {
		return Position{Offset: int(p)}
	}
	index := sort.Search(len(f.files), func(i int) bool {
		return f.files[i].End() >= int(p)
	})
	if index == len(f.files) {
		index--
	}
	return f.files[index].Position(p)
}
