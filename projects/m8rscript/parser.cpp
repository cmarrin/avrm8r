
#include "ucParser.h"

using namespace UC;

static uint8_t gL0[]  = { O_RSHIFTEQ, O_RSHIFTFILLEQ, O_LSHIFTEQ, O_ADDEQ, O_SUBEQ, O_MULEQ, O_DIVEQ, O_MODEQ, O_ANDEQ, O_XOREQ, O_OREQ, '=' , 0 };
static uint8_t gL2[]  = { O_LOR, 0 };
static uint8_t gL3[]  = { O_LAND , 0 };
static uint8_t gL4[]  = { '|', 0 };
static uint8_t gL5[]  = { '^', 0 };
static uint8_t gL6[]  = { '&', 0 };
static uint8_t gL7[]  = { O_EQ, O_NE, 0 };
static uint8_t gL8[]  = { '<', '>', O_LE, O_GE, K_IN, 0 };
static uint8_t gL9[]  = { O_LSHIFT, O_RSHIFT, O_RSHIFTFILL, 0 };
static uint8_t gL10[] = { '+', '-', 0 };
static uint8_t gL11[] = { '*', '/', '%', 0 };

uint8_t* Parser::mygOps[] = { gL0, NULL, gL2, gL3, gL4, gL5, gL6, gL7, gL8, gL9, gL10, gL11, NULL };

void
Parser::function(bool isStatement)
{
	if (isStatement) {
		if (tokenError(T_IDENTIFIER)) return;
		addAtom(T_IDENTIFIER);
		advance();
	}

	if (tokenError('(')) return;
	uint8_t n = formalParameterList();
	if (tokenError(')')) return;
	enterBlock();
	compoundStatement();
	add(K_RETURN);
	leaveBlock();
	add(K_FUNCTION);
	add(n);
	
	if (isStatement)
		add('=');
}
	
bool
Parser::statement()
{
	if (getToken() == K_IF) {
		advance();
		return ifStatement();
	}
	
	if (getToken() == K_VAR) {
		advance();
		varDeclList();
		return !tokenError(';');
		
	}
	
	if (compoundStatement())
		return true;
	
	if (getToken() == K_SWITCH) {
		advance();
		return switchStatement();
	}
	
	if (getToken() == K_FOR) {
		advance();
		return forStatement();
	}
	
	if (getToken() == K_FOREACH) {
		advance();
		return foreachStatement();
	}
	
	if (getToken() == K_DO) {
		advance();
		return doStatement();
	}
	
	if (getToken() == K_WHILE) {
		advance();
		return whileStatement();
	}

	if (getToken() == K_CONTINUE) {
		advance();
		add(K_CONTINUE);
		return !tokenError(';');
	}

	if (getToken() == K_BREAK) {
		advance();
		add(K_BREAK);
		return !tokenError(';');
	}

	if (getToken() == K_RETURN) {
		advance();
		if (!expression())
			add(K_NULL);
		add(K_RETURN);
		return !tokenError(';');
	}

	// try expression
	return expressionStatement();
}

bool
Parser::ifStatement()
{
	if (tokenError('(')) return false;
	advance();
	expression();
	if (tokenError(')')) return false;
	advance();
	enterBlock();
	statement();
	leaveBlock();
	if (getToken() == K_ELSE) {
		advance();
		enterBlock();
		statement();
		leaveBlock();
	}
	else
		addEmptyBlock();
	add(K_IF);
	return true;
}
	
bool
Parser::switchStatement()
{
	if (tokenError('(')) return false;
	advance();
	expression();
	if (tokenError(')')) return false;
	advance();
	if (tokenError('{')) return false;
	advance();
	enterBlock();

	bool haveDefault = false;
	while(1) {
		if (getToken() == K_CASE) {
			expression();
		}
		else if (getToken() == K_DEFAULT) {
			if (haveDefault)
				return false;
			haveDefault = true;
			add(K_DEFAULT);
		}
		else
			break;
		
		if (tokenError(':')) return false;
		statement();
		add(K_CASE);
	}

	leaveBlock();
	add(K_SWITCH);
	return true;
}

bool
Parser::forStatement()
{
	if (tokenError('(')) return false;
	advance();
	expressionStatement();
	enterBlock();
	expressionStatement();
	leaveBlock();
	enterBlock();
	expression();
	add(OP_POP);
	leaveBlock();
	if (tokenError(')')) return false;
	enterBlock();
	statement();
	leaveBlock();
	add(K_FOR);
	return true;
}

bool
Parser::foreachStatement()
{
	if (tokenError('(')) return false;
	advance();
	unaryExpression();
	if (tokenError(K_IN)) return false;
	expression();
	if (tokenError(')')) return false;
	enterBlock();
	statement();
	leaveBlock();
	add(K_FOREACH);
	return true;
}

bool
Parser::doStatement()
{
	if (!statement())
		return false;
	if (tokenError(K_WHILE)) return false;
	advance();
	if (tokenError('(')) return false;
	advance();
	if (!expression())
		return false;
	if (tokenError(')')) return false;
	advance();
	if (tokenError(';')) return false;
	advance();
	return true;
}

bool
Parser::whileStatement()
{
	if (tokenError('(')) return false;
	advance();
	if (!expression())
		return false;
	if (tokenError(')')) return false;
	advance();
	return !statement();
}
	
bool
Parser::primaryExpression()
{
	uint8_t token = getToken();
	if (token == T_IDENTIFIER) {
		addAtom(T_IDENTIFIER);
		add(OP_REF);
	}
	else if (token == T_STRING)
		addAtom(T_STRING);
	else if (token >= FIRST_CONST_TERM && token <= LAST_CONST_TERM)
		add(token);
	else if (token == T_FLOAT)
		addFloat();
	else if (token == T_INTEGER)
		addInt();
	else if (token == '(') {
		advance();
		expression();
		if (tokenError(')')) return false;
	}
	else if (token == K_FUNCTION) {
		advance();
		function(false);
	}
	else if (objectLiteral())
		return true;
	else {
		// error
		return false;
	}
	advance();
	return true;
}

bool
Parser::postfixExpression()
{
	primaryExpression();
	uint8_t op = getToken();
	if (op == '[') {
		advance();
		expression();
		if (tokenError(']')) return false;
		add('[');
	}
	else if (op == '(') {
		advance();
		uint8_t n = argumentExpressionList();
		if (tokenError(')')) return false;
		addFunctionCall(n);
	}
	else if (op == T_STRING) {
		addAtom(T_STRING);
		addFunctionCall(1);
	}
	else if (op == '.') {
		advance();
		if (tokenError(T_IDENTIFIER)) return false;
		addAtom(T_IDENTIFIER);
		add('[');
	}
	else if (op == O_INC) {
		add(OP_POST_INC);
	}
	else if (op == O_DEC) {
		add(OP_POST_DEC);
	}
	else if (objectLiteral()) {
		addFunctionCall(1);
		return true;
	}
	else
		return false;
	advance();
	return true;
}

bool
Parser::unaryExpression()
{
	uint8_t op = getToken();
	if (op == '+' || op == '-' || op == '!' || op == '~' || op == K_DELETE || 
					op == K_TYPEOF || op == O_INC || op == O_DEC || op == K_NEW) {
		advance();
		unaryExpression();
	}
	else
		op = C_EOF;
	if (!postfixExpression())
		return false;
	if (op != C_EOF)
		add(op);
	return true;
}
	
bool
Parser::expression(uint8_t level /*=0*/)
{
	if (level == 1) {
		if (!expression(2))
			return false;
			
		while (getToken() == '?') {
			advance();
			addJumpFalse(0);
			expression();
			addJump(1);
			resolveJump(0);
			if (tokenError(':')) return false;
			expression(2);
			resolveJump(1);
		}
		return true;
	}
	
	if (level == 12) {
		return unaryExpression();
	}
	
	if (!expression(level+1))
		return false;
		
	while(1) {
		uint8_t token = getToken();
		if (!op(level, token))
			break;
		advance();
		expression(level+1);
		add(token);
	}
	
	return true;
}
	
bool
Parser::tokenError(uint8_t token)
{
	return true;
}
