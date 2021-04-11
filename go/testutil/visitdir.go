/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * visit.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package testutil

import (
	"os"
	"path"
	"testing"

	"github.com/cosmos72/onejit/go/io"
)

type Opener = func() (filename string, src io.ReadCloser)

type DirVisitor = func(t *testing.T, fileopener Opener, dirname string)

func VisitDir(t *testing.T, visitor DirVisitor, dirname string) bool {
	return visitDirRecurse(t, visitor, dirname, false)
}

func VisitDirRecurse(t *testing.T, visitor DirVisitor, dirname string) bool {
	return visitDirRecurse(t, visitor, dirname, true)
}

func visitDirRecurse(t *testing.T, visitor DirVisitor, dirname string, recurse bool) bool {
	if len(dirname) > 512 {
		// try to avoid symlink loops
		t.Skip("path too long, aborting test: ", dirname)
		return false
	} else if expectInvalidSources(dirname) {
		t.Logf("         directory %q usually contains invalid sources, skipping it", dirname)
		return true
	}
	d, err := os.Open(dirname)
	if err != nil {
		t.Logf("error opening directory %q, skipping it: %v", dirname, err)
		return true
	}
	info, err := d.Readdir(0)
	d.Close()
	if err != nil {
		t.Logf("error reading directory %q: %v", dirname, err)
		return true
	}
	if testing.Verbose() {
		t.Logf("entering directory %q", dirname)
	}
	sortInfo(info)
	var filenames []string
	for _, entry := range info {
		name := entry.Name()
		filename := path.Join(dirname, name)
		if len(name) > 3 && name[len(name)-3:] == ".go" {
			if !entry.Mode().IsRegular() {
				// skip directories named '*.go'
				continue
			} else if existsDir(info, name[:len(name)-2]+"dir") {
				// skip file 'x.go' if there is a corresponding directory named 'x.dir'
				continue
			}
			filenames = append(filenames, filename)
		} else if recurse && entry.IsDir() {
			if !visitDirRecurse(t, visitor, filename, recurse) {
				return false
			}
		}
	}
	opener := makeFileOpener(t, filenames)
	visitor(t, opener, dirname)
	return true
}

func makeFileOpener(t *testing.T, filenames []string) Opener {
	i := 0
	return func() (filename string, src io.ReadCloser) {
		for i < len(filenames) {
			filename = filenames[i]
			f, err := os.Open(filename)
			i++
			if err != nil {
				t.Logf("error opening file %q, skipping it: %v", filename, err)
				continue
			} else if shouldSkipFile(f) {
				continue
			}
			return filename, f
		}
		return "", nil
	}
}
