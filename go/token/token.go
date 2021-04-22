/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * token.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

// equivalent to go/token.Token
type Token int

const (
	ILLEGAL Token = iota
	EOF
	COMMENT

	literal_beg
	IDENT
	INT
	FLOAT
	IMAG
	CHAR
	STRING
	literal_end

	operator_beg
	// Operators
	ADD
	SUB
	MUL
	QUO
	REM

	AND
	OR
	XOR
	SHL
	SHR
	AND_NOT

	ADD_ASSIGN
	SUB_ASSIGN
	MUL_ASSIGN
	QUO_ASSIGN
	REM_ASSIGN

	AND_ASSIGN
	OR_ASSIGN
	XOR_ASSIGN
	SHL_ASSIGN
	SHR_ASSIGN
	AND_NOT_ASSIGN

	LAND
	LOR
	ARROW
	INC
	DEC

	EQL
	LSS
	GTR
	ASSIGN
	NOT

	NEQ
	LEQ
	GEQ
	DEFINE
	ELLIPSIS

	LPAREN
	LBRACK
	LBRACE
	COMMA
	PERIOD

	RPAREN
	RBRACK
	RBRACE
	SEMICOLON
	COLON
	operator_end

	keyword_beg
	// Keywords
	BREAK
	CASE
	CHAN
	CONST
	CONTINUE
	DEFAULT
	DEFER
	ELSE
	FALLTHROUGH
	FOR
	FUNC
	GO
	GOTO
	IF
	IMPORT
	INTERFACE
	MAP
	PACKAGE
	RANGE
	RETURN
	SELECT
	STRUCT
	SWITCH
	TYPE
	VAR
	keyword_end

	// array or slice type. if first child is nil => slice type.
	// otherwise first child is array length (possibly '...')
	ARRAY
	BLOCK
	BOTH_DIR
	CALL
	COMPOSITE_LIT // type { elem0, elem1 ... }
	DECLS         // list of declarations
	DIR           // list of files
	EXPRS
	// struct field, interface element, function param or result.
	// stored in *ast.Field
	FIELD
	FILE
	GENERIC     // generic type parameters [T0 i0, T1 i1, ...]
	IMPORT_SPEC // single import line 'alias "full/pkg/path"'
	IMPORTS     // list of IMPORT nodes
	INDEX       // array/map/slice indexing a[b] or generic instantiation a[b,c...]
	KEY_VALUE   // key: value inside a composite literal
	LABEL       // label: statement
	LAMBDA      // function literal
	NAMES       // list of identifiers
	PARAMS
	RECV_DIR
	RESULTS
	SEND_DIR
	SLICE // slice expression a[b:c] or a[b:c:d]
	TYPE_ASSERT
	TYPES      // list of types. used by generics in 'interface { type /* typelist */ }'
	TYPESWITCH // switch x.(type) { }
	VALUE_SPEC
)

var tokens = [...]string{
	ILLEGAL: "ILLEGAL",
	EOF:     "EOF",
	COMMENT: "COMMENT",

	IDENT:  "IDENT",
	INT:    "INT",
	FLOAT:  "FLOAT",
	IMAG:   "IMAG",
	CHAR:   "CHAR",
	STRING: "STRING",

	ADD: "+",
	SUB: "-",
	MUL: "*",
	QUO: "/",
	REM: "%",

	AND:     "&",
	OR:      "|",
	XOR:     "^",
	SHL:     "<<",
	SHR:     ">>",
	AND_NOT: "&^",

	ADD_ASSIGN: "+=",
	SUB_ASSIGN: "-=",
	MUL_ASSIGN: "*=",
	QUO_ASSIGN: "/=",
	REM_ASSIGN: "%=",

	AND_ASSIGN:     "&=",
	OR_ASSIGN:      "|=",
	XOR_ASSIGN:     "^=",
	SHL_ASSIGN:     "<<=",
	SHR_ASSIGN:     ">>=",
	AND_NOT_ASSIGN: "&^=",

	LAND:  "&&",
	LOR:   "||",
	ARROW: "<-",
	INC:   "++",
	DEC:   "--",

	EQL:    "==",
	LSS:    "<",
	GTR:    ">",
	ASSIGN: "=",
	NOT:    "!",

	NEQ:      "!=",
	LEQ:      "<=",
	GEQ:      ">=",
	DEFINE:   ":=",
	ELLIPSIS: "...",

	LPAREN: "(",
	LBRACK: "[",
	LBRACE: "{",
	COMMA:  ",",
	PERIOD: ".",

	RPAREN:    ")",
	RBRACK:    "]",
	RBRACE:    "}",
	SEMICOLON: ";",
	COLON:     ":",

	BREAK:       "break",
	CASE:        "case",
	CHAN:        "chan",
	CONST:       "const",
	CONTINUE:    "continue",
	DEFAULT:     "default",
	DEFER:       "defer",
	ELSE:        "else",
	FALLTHROUGH: "fallthrough",
	FOR:         "for",
	FUNC:        "func",
	GO:          "go",
	GOTO:        "goto",
	IF:          "if",
	IMPORT:      "import",
	INTERFACE:   "interface",
	MAP:         "map",
	PACKAGE:     "package",
	RANGE:       "range",
	RETURN:      "return",
	SELECT:      "select",
	STRUCT:      "struct",
	SWITCH:      "switch",
	TYPE:        "type",
	VAR:         "var",

	ARRAY:         "ARRAY",
	BLOCK:         "BLOCK",
	BOTH_DIR:      "BOTH_DIR",
	CALL:          "CALL",
	COMPOSITE_LIT: "COMPOSITE_LIT",
	DIR:           "DIR",
	DECLS:         "DECLS",
	EXPRS:         "EXPRS",
	FIELD:         "FIELD",
	FILE:          "FILE",
	GENERIC:       "GENERIC",
	IMPORT_SPEC:   "IMPORT_SPEC",
	IMPORTS:       "IMPORTS",
	INDEX:         "INDEX",
	KEY_VALUE:     "KEY_VALUE",
	LABEL:         "LABEL",
	LAMBDA:        "LAMBDA",
	NAMES:         "NAMES",
	PARAMS:        "PARAMS",
	RECV_DIR:      "RECV_DIR",
	RESULTS:       "RESULTS",
	SEND_DIR:      "SEND_DIR",
	SLICE:         "SLICE",
	TYPE_ASSERT:   "TYPE_ASSERT",
	TYPES:         "TYPES",
	TYPESWITCH:    "TYPESWITCH",
	VALUE_SPEC:    "VALUE_SPEC",
}

var (
	operators map[string]Token = makeTokenMap(operator_beg, operator_end)
	keywords  map[string]Token = makeTokenMap(keyword_beg, keyword_end)
)

func makeTokenMap(beg, end Token) map[string]Token {
	lo, hi := beg+1, end-1
	m := make(map[string]Token, hi-lo+1)
	for op := lo; op <= hi; op++ {
		m[op.String()] = op
	}
	return m
}

func LookupOperator(str string) Token {
	return operators[str]
}

func Lookup(str string) Token {
	n := len(str)
	if n < 2 || n > 11 { // len("fallthrough") == 11
		return IDENT
	}
	tok := keywords[str]
	if tok == ILLEGAL {
		tok = IDENT
	}
	return tok
}

func (tok Token) IsKeyword() bool {
	return tok > keyword_beg && tok < keyword_end
}

func (tok Token) IsLiteral() bool {
	return tok > literal_beg && tok < literal_end
}

func (tok Token) IsOperator() bool {
	return tok > operator_beg && tok < operator_end
}

const (
	LowestPrec  = 0 // non-operators
	UnaryPrec   = 6
	HighestPrec = 7
)

func (tok Token) Precedence() int {
	switch tok {
	case LOR:
		return 1
	case LAND:
		return 2
	case EQL, NEQ, LSS, LEQ, GTR, GEQ:
		return 3
	case ADD, SUB, OR, XOR:
		return 4
	case MUL, QUO, REM, SHL, SHR, AND, AND_NOT:
		return 5
	}
	return LowestPrec
}

func (tok Token) String() string {
	var ret string
	if tok >= 0 && int(tok) < len(tokens) {
		ret = tokens[tok]
	}
	if len(ret) == 0 {
		ret = "UNKNOWN"
	}
	return ret
}
