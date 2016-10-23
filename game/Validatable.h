//
// Created by serg on 10/22/16.
//

#ifndef TIC_TAC_TOE_VALIDATABLE_H
#define TIC_TAC_TOE_VALIDATABLE_H


class Validatable
{
public:
    Validatable() : _is_valid(false)
    {}

    virtual ~Validatable() = default;

    bool isValid()
    { return _is_valid; }

    void setValid(bool is_valid)
    { _is_valid = is_valid; }

private:
    bool _is_valid;
};


#endif //TIC_TAC_TOE_VALIDATABLE_H
