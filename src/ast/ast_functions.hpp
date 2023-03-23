#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_expression.hpp"

#include <cmath>


class FunctionDef
    : public Expression
{
private:
    std::string returnType;
    std::string variableName;
    ExpressionPtr args;
    ExpressionPtr functionBody;
protected:
    
public:
    FunctionDef(std::string *_returnType, std::string *_variableName, ExpressionPtr _args, ExpressionPtr _functionBody){
        returnType =*_returnType;
        variableName = *_variableName;
        args = _args;
        functionBody = _functionBody;
    }
    virtual ~FunctionDef(){
        delete args;
        delete functionBody;
    }
    virtual void codeGen(std::ostream &dst, std::string destreg) const override{
        dst << ".text" << std::endl;
        dst << ".globl " <<  variableName << std::endl;
        dst << variableName << ":" << std::endl;
        dst << "addi sp,sp,-16" << std::endl;
        dst << "sw s0,12(sp)" << std::endl;
        dst << "addi s0,sp,16" << std::endl;
        functionBody->codeGen(dst, destreg);
    }
};

class Return
    : public Expression
{
private:
    ExpressionPtr returnVal;

public:
    Return(ExpressionPtr _returnVal)
        : returnVal(_returnVal)
    {}
    virtual ~Return(){
        delete returnVal;
    }
    virtual void codeGen(std::ostream &dst, std::string destreg) const override{

        returnVal->codeGen(dst, "a5");
        dst << "mv " << destreg << ",a5" << std::endl;
        dst << "lw s0,12(sp)" << std::endl;
        dst << "addi sp,sp,16" << std::endl;
        dst << "jr ra" << std::endl;
    }
};



class Function
    : public Expression
{
private:
    ExpressionPtr arg;
protected:
    Function(ExpressionPtr _arg)
        : arg(_arg)
    {}
public:
    virtual ~Function()
    {
        delete arg;
    }

    virtual const char * getFunction() const =0;

    ExpressionPtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<getFunction()<<"( ";
        arg->print(dst);
        dst<<" )";
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // NOTE : This should be implemented by the inheriting function nodes, e.g. LogFunction
        throw std::runtime_error("FunctionOperator::evaluate is not implemented.");
    }
};

class LogFunction
    : public Function
{
public:
    LogFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "log"; }
    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        return std::log(getArg()->evaluate(bindings));
    }
    // TODO-E : Override evaluate, and implement it
};

class ExpFunction
    : public Function
{
public:
    ExpFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "exp"; }
    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override {
        return std::exp(getArg()->evaluate(bindings));
    }
};

class SqrtFunction
    : public Function
{
public:
    SqrtFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "sqrt"; }
    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override {
        return std::sqrt(getArg()->evaluate(bindings));
    }
};


#endif
