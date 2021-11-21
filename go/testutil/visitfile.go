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
	"bytes"
	"io/ioutil"
	"os"
	"path"
	"testing"

	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/sort"
	"github.com/cosmos72/onejit/go/strings"
)

type FileVisitor = func(t *testing.T, in io.Reader, filename string)

func VisitFile(t *testing.T, visit FileVisitor, filename string) {
	f, err := os.Open(filename)
	if err != nil {
		t.Logf("error opening file %q, skipping it: %v", filename, err)
		return
	}
	defer f.Close()
	if !shouldSkipFile(f) {
		visit(t, f, filename)
	}
}

func shouldSkipFile(f *os.File) bool {
	b, _ := ioutil.ReadAll(f)
	if bytes.Contains(b, []byte("ERROR")) {
		// file is supposed to contain some kind of error, skip it
		return true
	}
	f.Seek(0, 0)
	return false
}

func VisitFilesRecurse(t *testing.T, visitor FileVisitor, dirname string) bool {
	if len(dirname) > 512 {
		// try to avoid symlink loops
		t.Skip("path too long, aborting test: ", dirname)
		return false
	} else if expectInvalidSources(dirname) {
		t.Logf("         directory %q usually contains invalid/unsupported sources, skipping it", dirname)
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
			VisitFile(t, visitor, filename)
		} else if entry.IsDir() {
			if !VisitFilesRecurse(t, visitor, filename) {
				return false
			}
		}
	}
	return true
}

func existsDir(info []os.FileInfo, dirname string) bool {
	pos := sort.Search(len(info), func(i int) bool {
		return info[i].Name() >= dirname
	})
	return pos < len(info) && info[pos].Name() == dirname
}

func expectInvalidSources(dirname string) bool {
	name := strings.Basename(dirname)
	// these directories contain invalid sources at least in Go 1.13.15
	return name == "badpkg" || name == "notest" || name == "syntaxerror" ||
		// this directory contains sources that use Go generics - not yet supported
		name == "typeparam"
}
