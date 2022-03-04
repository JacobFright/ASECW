#include "changepassword.h"
#include "ui_changepassword.h"

/**
 * @brief ChangePassword::ChangePassword
 * @param parent
 */
ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
}

/**
 * @brief ChangePassword::~ChangePassword
 */
ChangePassword::~ChangePassword()
{
    delete ui;
}
