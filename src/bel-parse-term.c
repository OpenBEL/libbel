
/* #line 1 "bel-parse-term.rl" */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "libbel.h"
#include "bel-ast.h"
#include "bel-node-stack.h"


/* #line 13 "bel-parse-term.c" */
static const int set_start = 2;
static const int set_first_final = 2;
static const int set_error = -1;

static const int set_en_term = 2;


/* #line 12 "bel-parse-term.rl" */


/* private */
bel_ast* _create_term_ast();

/* private */
bel_ast_node* _create_term_ast_node();

/* private */
bel_ast_node* _create_wildcard_arg_ast_node();

/* private */
bel_ast_node* _mutable_node_value(bel_ast_value_type type, char* value);

/* private */
bel_ast_node* _set_wildcard_as_term_node(bel_ast_node* wildcard_node);

/* private */
bel_ast_node* _set_wildcard_as_nv_node(bel_ast_node* node);

/* private */
void _swap_left_right(bel_ast_node* wildcard_node);

/* private */
void _mark_subject_node_complete(bel_ast_node* statement_node);

/* private */
void _mark_object_node_complete(bel_ast_node* statement_node);

/* private */
void _mark_statement_node_complete(bel_ast_node* statement_node);

bel_ast* bel_parse_term(char* line) {
    // ragel - definitions
    int             cs;
    char            *p;
    char            *pe;
    int             *stack;
    char            *ts;
    char            *te;
    char            *eof;
    int             act;

    // state - definitions
    bel_ast*        ast;
    bel_node_stack* arg_stack;
    bel_ast_node*   wildcard_node;
    bel_ast_node*   arg;
    bel_ast_node*   term;
    char            *token;
    int             ti;
    int             whitespace_count;

    // Copy line to C stack; Append new line if needed.
    int             line_length = strlen(line);
    int             i;
    char            input[line_length + 2];
    strcpy(input, line);
    for (i = line_length - 1; (input[i] == '\n' || input[i] == '\r'); i--)
        input[i] = '\0';
    input[i + 1] = '\n';
    input[i + 2] = '\0';

    // ragel - assignments
    p                 = input;
    pe                = input + strlen(input);
    eof               = pe;
    stack             = malloc(sizeof(int) * STACK_SIZE);

    // state - initial assignments
    ast               = bel_new_ast();
    arg_stack         = stack_init(STACK_SIZE);
    wildcard_node     = NULL;
    arg               = NULL;
    term              = NULL;
    ast->root         = wildcard_node;
    token             = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
    ti                = 0;
    whitespace_count   = 0;
    memset(token, '\0', BEL_VALUE_CHAR_LEN);

    
/* #line 104 "bel-parse-term.c" */
	{
	cs = set_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 112 "bel-parse-term.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
/* #line 139 "bel-parse-term.rl" */
	{te = p+1;{
            wildcard_node->token->right->value->start_position = (ts - input) - whitespace_count;
            wildcard_node->token->right->value->end_position   = (te - input) - whitespace_count;
            wildcard_node->token->is_complete = 1;
            arg = stack_peek(arg_stack);
            arg->token->is_complete = 1;
        }}
	goto st2;
tr4:
/* #line 231 "bel-parse-term.rl" */
	{te = p+1;{
            whitespace_count++;
        }}
	goto st2;
tr5:
/* #line 194 "bel-parse-term.rl" */
	{te = p+1;{
            if (wildcard_node && wildcard_node->token->ttype == BEL_TOKEN_ARG) {
                _set_wildcard_as_nv_node(wildcard_node);
                wildcard_node->token->is_complete = 1;
            }
        }}
	goto st2;
tr7:
/* #line 147 "bel-parse-term.rl" */
	{te = p+1;{
            _set_wildcard_as_term_node(wildcard_node);
            wildcard_node->token->is_complete = 0;
            term = wildcard_node;
            stack_push(arg_stack, term->token->right);
            wildcard_node = NULL;
        }}
	goto st2;
tr8:
/* #line 155 "bel-parse-term.rl" */
	{te = p+1;{
            // mark term complete
            term->token->is_complete = 1;

            // if wildcard is ARG; then set as NV and mark complete
            if (wildcard_node && wildcard_node->token->ttype == BEL_TOKEN_ARG) {
                _set_wildcard_as_nv_node(wildcard_node);
                wildcard_node->token->is_complete = 1;
            }

            for (i = (arg_stack->top); i > -1; i--) {
                arg = arg_stack->contents[i];
                if (arg->token->left == term) {
                    arg->token->is_complete = 1;
                    break;
                }
            }

            stack_pop(arg_stack);
            while (!stack_is_empty(arg_stack)) {
                arg = stack_pop(arg_stack);
                if (arg->token->left
                      && arg->token->left->type_info->type == BEL_TOKEN
                      && arg->token->left->token->ttype == BEL_TOKEN_TERM
                      && arg->token->left != term) {
                    stack_push(arg_stack, arg);
                    term = arg->token->left;
                    arg  = term->token->right;
                    break;
                }
            }

            while (arg->token->left || arg->token->right) {
                arg = arg->token->right;
            }
            stack_push(arg_stack, arg);
            wildcard_node = NULL;
        }}
	goto st2;
tr9:
/* #line 215 "bel-parse-term.rl" */
	{te = p+1;{
            if (wildcard_node && wildcard_node->token->ttype == BEL_TOKEN_ARG) {
                _set_wildcard_as_nv_node(wildcard_node);
                wildcard_node->token->is_complete = 1;
            }

            arg = stack_peek(arg_stack);
            if (arg->token->left || arg->token->right) {
                while (arg->token->left || arg->token->right) {
                    arg = arg->token->right;
                }
                stack_push(arg_stack, arg);
            }
            wildcard_node = NULL;
        }}
	goto st2;
tr10:
/* #line 201 "bel-parse-term.rl" */
	{te = p+1;{
            _swap_left_right(wildcard_node);

            // INCOMPLETE
            if (wildcard_node) {
                wildcard_node->token->is_complete = 0;
            }
            arg = stack_peek(arg_stack);
            arg->token->is_complete = 0;

            // A2NV
            _set_wildcard_as_nv_node(wildcard_node);
        }}
	goto st2;
tr11:
/* #line 139 "bel-parse-term.rl" */
	{te = p;p--;{
            wildcard_node->token->right->value->start_position = (ts - input) - whitespace_count;
            wildcard_node->token->right->value->end_position   = (te - input) - whitespace_count;
            wildcard_node->token->is_complete = 1;
            arg = stack_peek(arg_stack);
            arg->token->is_complete = 1;
        }}
	goto st2;
st2:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 271 "bel-parse-term.c" */
	switch( (*p) ) {
		case 9: goto tr4;
		case 10: goto tr5;
		case 32: goto tr4;
		case 34: goto tr6;
		case 40: goto tr7;
		case 41: goto tr8;
		case 44: goto tr9;
		case 58: goto tr10;
	}
	goto tr3;
tr12:
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
	goto st3;
tr3:
/* #line 94 "bel-parse-term.rl" */
	{
            ti = 0;
            ts = p;
            memset(token, '\0', BEL_VALUE_CHAR_LEN);
        }
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 347 "bel-parse-term.c" */
	switch( (*p) ) {
		case 32: goto tr11;
		case 34: goto tr11;
		case 44: goto tr11;
		case 58: goto tr11;
	}
	if ( (*p) > 10 ) {
		if ( 40 <= (*p) && (*p) <= 41 )
			goto tr11;
	} else if ( (*p) >= 9 )
		goto tr11;
	goto tr12;
tr0:
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
	goto st0;
tr6:
/* #line 94 "bel-parse-term.rl" */
	{
            ti = 0;
            ts = p;
            memset(token, '\0', BEL_VALUE_CHAR_LEN);
        }
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
	goto st0;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
/* #line 424 "bel-parse-term.c" */
	switch( (*p) ) {
		case 34: goto tr1;
		case 92: goto tr2;
	}
	goto tr0;
tr2:
/* #line 100 "bel-parse-term.rl" */
	{
            te = p;
            if (!wildcard_node) {
                wildcard_node = _create_wildcard_arg_ast_node();

                if (!ast->root) {
                    ast->root = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    ast->root->token->left  = wildcard_node;
                    ast->root->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                    stack_push(arg_stack, ast->root);
                } else {
                    arg = stack_peek(arg_stack);
                    arg->token->left  = wildcard_node;
                    arg->token->right = bel_new_ast_node_token(BEL_TOKEN_ARG);
                }
            }

            if (!wildcard_node->token->right->value->value) {
                wildcard_node->token->right->value->value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
                memset(wildcard_node->token->right->value->value, '\0', BEL_VALUE_CHAR_LEN);
            }

            wildcard_node->token->right->value->value[ti++] = (*p);
        }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 461 "bel-parse-term.c" */
	if ( (*p) == 92 )
		goto tr2;
	goto tr0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3: goto tr11;
	}
	}

	}

/* #line 248 "bel-parse-term.rl" */


    if (!ast->root) {
        ast->root = bel_new_ast_node_token(BEL_TOKEN_NULL);
    } else {
        arg = ast->root->token->left;
        if (arg->type_info->type == BEL_TOKEN && arg->token->ttype == BEL_TOKEN_TERM) {
          arg = bel_copy_ast_node(arg);
          bel_free_ast_node(ast->root);
          ast->root = arg;
        }
    }

    // free allocations
    if (arg_stack) {
        stack_destroy(arg_stack);
    }
    free(stack);

    return ast;
};

/* private */
bel_ast* _create_term_ast() {
    bel_ast*      ast;
    bel_ast_node* term_node;

    ast       = bel_new_ast();
    term_node = _create_term_ast_node();
    ast->root = term_node;

    return ast;
};

/* private */
bel_ast_node* _create_term_ast_node() {
    bel_ast_node* term;
    bel_ast_node* fx_node;
    bel_ast_node* arg;
    char*         fx_value;

    fx_value = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
    memset(fx_value, '\0', BEL_VALUE_CHAR_LEN);

    term     = bel_new_ast_node_token(BEL_TOKEN_TERM);
    fx_node  = _mutable_node_value(BEL_VALUE_FX, fx_value);
    arg      = bel_new_ast_node_token(BEL_TOKEN_ARG);

    term->token->left  = fx_node;
    term->token->right = arg;

    return term;
};

/* private */
bel_ast_node* _create_wildcard_arg_ast_node() {
    bel_ast_node* arg;
    bel_ast_node* value_node_1;
    bel_ast_node* value_node_2;
    char*         value_1;
    char*         value_2;

    value_1 = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
    value_2 = malloc(sizeof(char) * BEL_VALUE_CHAR_LEN);
    memset(value_1, '\0', BEL_VALUE_CHAR_LEN);
    memset(value_2, '\0', BEL_VALUE_CHAR_LEN);

    arg          = bel_new_ast_node_token(BEL_TOKEN_ARG);
    value_node_1 = _mutable_node_value(BEL_VALUE_VAL, value_1);
    value_node_2 = _mutable_node_value(BEL_VALUE_VAL, value_2);

    arg->token->left  = value_node_1;
    arg->token->right = value_node_2;

    return arg;
};

/* private */
bel_ast_node* _mutable_node_value(bel_ast_value_type type, char* value) {
    bel_ast_node* node;

    node = malloc(sizeof(bel_ast_node));
    node->value = malloc(sizeof(bel_ast_node_value));
    node->value->type  = BEL_VALUE;
    node->value->vtype = type;
    node->value->value = value;
    return node;
};

bel_ast_node* _set_wildcard_as_term_node(bel_ast_node* wildcard_node) {
    bel_ast_node* fx_node;
    bel_ast_node* arg_node;
    wildcard_node->token->ttype              = BEL_TOKEN_TERM;
    wildcard_node->token->left->value->vtype = BEL_VALUE_FX;

    // Convert wildcard to TERM:
    // - Move value pointer from right to left.
    fx_node = bel_new_ast_node_value(
        BEL_VALUE_FX,
        wildcard_node->token->right->value->value,
        wildcard_node->token->right->value->start_position,
        wildcard_node->token->right->value->end_position
    );

    bel_free_ast_node(wildcard_node->token->left);
    bel_free_ast_node(wildcard_node->token->right);

    wildcard_node->token->left = fx_node;
    arg_node = bel_new_ast_node_token(BEL_TOKEN_ARG);
    wildcard_node->token->right = arg_node;

    return wildcard_node;
};

bel_ast_node* _set_wildcard_as_nv_node(bel_ast_node* wildcard_node) {
    wildcard_node->token->ttype               = BEL_TOKEN_NV;
    wildcard_node->token->left->value->vtype  = BEL_VALUE_PFX;
    wildcard_node->token->right->value->vtype = BEL_VALUE_VAL;

    if (wildcard_node->token->left->value->value[0] == '\0') {
        free(wildcard_node->token->left->value->value);
        wildcard_node->token->left->value->value = NULL;
    }

    if (wildcard_node->token->right->value->value[0] == '\0') {
        free(wildcard_node->token->right->value->value);
        wildcard_node->token->right->value->value = NULL;
    }

    return wildcard_node;
};

void _swap_left_right(bel_ast_node* wildcard_node) {
    bel_ast_node* left;

    left = wildcard_node->token->left;
    wildcard_node->token->left  = wildcard_node->token->right;
    wildcard_node->token->right = left;
};

void _mark_subject_node_complete(bel_ast_node* statement_node) {
    bel_ast_node* subject_node;
    bel_ast_node* term_node;

    assert(statement_node != NULL);

    subject_node   = statement_node->token->left;
    term_node      = subject_node->token->left;

    if (term_node && term_node->token->is_complete) {
        subject_node->token->is_complete = 1;
    }
};

void _mark_object_node_complete(bel_ast_node* statement_node) {
    bel_ast_node* object_node;
    bel_ast_node* rel_node;
    bel_ast_node* object_term_or_statement_node;

    assert(statement_node != NULL);

    object_node                   = statement_node->token->right;
    rel_node                      = object_node->token->left;
    object_term_or_statement_node = object_node->token->right;

    if (rel_node && rel_node->token->is_complete &&
        (object_term_or_statement_node && object_term_or_statement_node->token->is_complete)) {

        object_node->token->is_complete = 1;
    }
};

void _mark_statement_node_complete(bel_ast_node* statement_node) {
    bel_ast_node* subject_node;
    bel_ast_node* object_node;

    assert(statement_node != NULL);

    subject_node                  = statement_node->token->left;
    // SUBJECT is not complete implies the STATEMENT is not complete
    if (!subject_node->token->is_complete) {
        statement_node->token->is_complete = 0;
        return;
    }

    object_node                   = statement_node->token->right;

    // OBJECT L:NULL R:NULL implies the STATEMENT is subject-only and complete
    if (object_node->token->left == NULL && object_node->token->right == NULL) {
        statement_node->token->is_complete = 1;
        return;
    }

    if (!object_node->token->is_complete) {
        statement_node->token->is_complete = 0;
        return;
    }

    // SUBJECT is complete and OBJECT L:REL R:TERM|STATEMENT is complete
    statement_node->token->is_complete = 1;
};

// vim: ft=ragel sw=4 ts=4 sts=4 expandtab

