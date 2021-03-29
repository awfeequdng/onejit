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
	"io"
	"io/ioutil"
	"os"
	"path"
	"sort"
	"testing"
)

type Visit = func(t *testing.T, in io.Reader, filename string)

func VisitDirRecurse(t *testing.T, visit Visit, dirname string) bool {
	if len(dirname) > 512 {
		// try to avoid symlink loops
		t.Skip("path too long, aborting test: ", dirname)
		return false
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
		name := path.Join(dirname, entry.Name())
		if entry.IsDir() {
			if !VisitDirRecurse(t, visit, name) {
				return false
			}
		} else if entry.Mode().IsRegular() &&
			len(name) > 3 && name[len(name)-3:] == ".go" {

			VisitFile(t, visit, name)
		}
	}
	return true
}

func sortInfo(info []os.FileInfo) {
	sort.Slice(info, func(i int, j int) bool {
		return info[i].Name() < info[j].Name()
	})
}

func VisitFile(t *testing.T, visit Visit, filename string) {
	file, err := os.Open(filename)
	if err != nil {
		t.Logf("error opening file %q, skipping it: %v", filename, err)
		return
	}
	defer file.Close()
	b, _ := ioutil.ReadAll(file)
	if bytes.Contains(b, []byte("ERROR")) {
		// file is supposed to contain some kind of error, skip it
		return
	}
	file.Seek(0, 0)
	visit(t, file, filename)
}
