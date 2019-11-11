///参变量(4)

#include "_4_Parametric_variable.h"

_4_Parametric_variable::_4_Parametric_variable(QWidget *parent) : QDialog(parent),
                                                                  ui(new Ui::_4_Parametric_variableForm)
{
    ui->setupUi(this);
    this->setWindowTitle("参变量");
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("Database.db");
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    att();
    act();
    ui->treeWidget_2->setColumnCount(7);
    ui->treeWidget_2->setHeaderLabels(QStringList() << "数据名称" << "属性" << "数值" << "数据类型" << "小数" << "单位" << "备注");
    ui->treeWidget_2->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeWidget_4->setColumnCount(7);
    ui->treeWidget_4->setHeaderLabels(QStringList() << "数据名称" << "属性" << "数值" << "数据类型" << "小数" << "单位" << "备注");
    ui->treeWidget_4->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    Item_box = {};
    ARRAY_box = {};
    pre_stat = {};
    connect(ui->treeWidget_2, SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int)), this,
            SLOT(EditFileFilter(QTreeWidgetItem * , int)));

    connect(ui->treeWidget_2, SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem * )), this,
            SLOT(itemchanged(QTreeWidgetItem * , QTreeWidgetItem * )));

    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(check_cancel()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(list_clear()));
}

void _4_Parametric_variable::att()
{
    ui->treeWidget->setColumnCount(4);
    ui->treeWidget->setHeaderLabels(QStringList() << "标识" << "名称" << "属性" << "读写");
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QSqlQuery sql_query;
    QSqlQuery sql_querychild;
    sql_query.exec("select * from oi_des where OITypeID=5 ");
    if (!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    } else
    {
        while (sql_query.next())
        {
            QString oiid = sql_query.value(0).toString();
            QString detail = sql_query.value(2).toString();
            QTreeWidgetItem *OI = new QTreeWidgetItem(ui->treeWidget);
            OI->setText(0, detail);
            OI->setText(1, sql_query.value(3).toString());
            sql_querychild.exec("select * from oi_attribute where OIID=" + oiid + " ORDER BY AttrNo");
            while (sql_querychild.next())
            {
                QString name = sql_querychild.value(3).toString();
                QString AttNo = sql_querychild.value(2).toString();
                QString RW = sql_querychild.value(5).toString();
                QTreeWidgetItem *OIchild = new QTreeWidgetItem(OI);
                OIchild->setText(1, name);
                OIchild->setText(2, AttNo);
                OIchild->setText(3, RW);
                OIchild->setCheckState(0, Qt::Unchecked);
            }
        }
    }
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem * , int)), this,
            SLOT(deal_box_att(QTreeWidgetItem * )));

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem * )),
            this, SLOT(add_item(QTreeWidgetItem * , QTreeWidgetItem * )));
}

void _4_Parametric_variable::act()
{
    ui->treeWidget_3->setColumnCount(3);
    ui->treeWidget_3->setHeaderLabels(QStringList() << "标识" << "名称" << "方法");
    ui->treeWidget_3->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QSqlQuery sql_query;
    QSqlQuery sql_querychild;
    sql_query.exec("select * from oi_des where OITypeID=5 ");
    if (!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    } else
    {
        while (sql_query.next())
        {
            QString oiid = sql_query.value(0).toString();
            QString detail = sql_query.value(2).toString();
            QTreeWidgetItem *OI = new QTreeWidgetItem(ui->treeWidget_3);
            OI->setText(0, detail);
            OI->setText(1, sql_query.value(3).toString());
            sql_querychild.exec("select * from oi_action where OIID=" + oiid + " ORDER BY ActionNo");
            while (sql_querychild.next())
            {
                QString name = sql_querychild.value(3).toString();
                QString AttNo = sql_querychild.value(2).toString();
                QString RW = sql_querychild.value(5).toString();
                QTreeWidgetItem *OIchild = new QTreeWidgetItem(OI);
                OIchild->setText(1, name);
                OIchild->setText(2, AttNo);
                OIchild->setText(3, RW);
                OIchild->setCheckState(0, Qt::Unchecked);
            }
        }
    }
    connect(ui->treeWidget_3, SIGNAL(itemChanged(QTreeWidgetItem * , int)), this,
            SLOT(deal_box_act(QTreeWidgetItem * )));

    connect(ui->treeWidget_3, SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem * )),
            this, SLOT(add_item(QTreeWidgetItem * , QTreeWidgetItem * )));
}

void _4_Parametric_variable::deal_box_att(QTreeWidgetItem *item)
{
    PARAMETRIC *need_del = nullptr;
    int i = -1;
            foreach(auto
                            x, Item_box)
        {
            i += 1;
            if (x.left == item)
            {
                flag = true;
                need_del = &x;
                break;
            }
        }
    if (!flag)
    {
        QSqlQuery sql_query;
        sql_query.exec("select * from oi_des where OI=" + item->parent()->text(0));
        sql_query.next();
        QString OIID = sql_query.value(0).toString();

        QSqlQuery sql_query2;
        sql_query2.exec("select * from oi_attribute where OIID= " + OIID + " and AttrNo = " + item->text(2));
        sql_query2.next();
        QString TypeCode = sql_query2.value(4).toString();

        QSqlQuery sql_query3;
        sql_query3.exec("select * from attr_detail where TypeCode= " + TypeCode);
        sql_query3.next();
        QString Typecode = sql_query3.value(1).toString();
        QString seq = sql_query3.value(3).toString();
        QString ItemVal = sql_query3.value(5).toString();//数据值
        QString DataType = sql_query3.value(6).toString();//数据类型
        QString Idx = sql_query3.value(7).toString();
        QString Dot = sql_query3.value(8).toString();//小数
        QString Unit = sql_query3.value(9).toString();//单位
        QString Tremark = sql_query3.value(10).toString();//备注
//        int EnEdit = sql_query3.value(11).toBool();

        auto newitem = new QTreeWidgetItem(ui->treeWidget_2);
        newitem->setText(0, item->parent()->text(0) + " " + item->parent()->text(1) + " " + item->text(1));
        newitem->setText(1, item->text(2));//属性
        newitem->setText(2, ItemVal);//数据值
        newitem->setText(3, DataType);//数据类型
        newitem->setText(4, Dot);//小数
        newitem->setText(5, Unit);//单位
        newitem->setText(6, Tremark);//备注
        newitem->setCheckState(0, Qt::Checked);
        if (DataType == "array")
        {
            MENBER_ARRAY array;
            array.ItemName = item->parent()->text(0) + " " + item->parent()->text(1) + " " + item->text(1);
            array.TypeCode = TypeCode;
            ARRAY_box.append(array);
        }
        if (DataType == "structure")
        {
            analy_database(newitem, Typecode);
        }
        PARAMETRIC new_m_itembox;
        new_m_itembox.left = item;
        new_m_itembox.right = newitem;
        Item_box.append(new_m_itembox);
    } else
    {
        delete need_del->right;
//        delete need_del;
        Item_box.removeAt(i);
        flag = false;
    }
}

void _4_Parametric_variable::analy_database(QTreeWidgetItem *item, QString Typecode)
{
    QSqlQuery sql_query5;
    sql_query5.exec("select * from attr_detail where PTypeCode=" + Typecode);
    while (sql_query5.next())
    {
        auto NewChildItem = new QTreeWidgetItem(item);
        QString name = sql_query5.value(4).toString();
        QString data_value = sql_query5.value(5).toString();
        QString data_type = sql_query5.value(6).toString();
        QString note = sql_query5.value(8).toString();
        QString unit = sql_query5.value(9).toString();
        QString remark = sql_query5.value(10).toString();
        NewChildItem->setText(0, name);//name
        NewChildItem->setText(2, data_value);//数据值
        NewChildItem->setText(3, data_type);//数据类型
        NewChildItem->setText(4, note);//小数
        NewChildItem->setText(5, unit);//单位
        NewChildItem->setText(6, remark);//备注
        if (data_type == "array")
        {
            MENBER_ARRAY array;
            if (item->parent())
                array.ItemName = item->parent()->text(0) + " " + item->parent()->text(1) + " " + item->text(1);
            else
                array.ItemName = name;
            array.TypeCode = sql_query5.value(1).toString();
            ARRAY_box.append(array);
        }
        if (data_type == "structure")
        {
            analy_database(NewChildItem, Typecode);
        }
    }
}

void _4_Parametric_variable::deal_box_act(QTreeWidgetItem *item)
{
    PARAMETRIC *need_del = nullptr;
    int i = -1;
            foreach(auto
                            x, Item_box)
        {
            i += 1;
            if (x.left == item)
            {
                flag = true;
                need_del = &x;
                break;
            }
        }
    if (!flag)
    {
        QSqlQuery sql_query;
        sql_query.exec("select * from oi_des where OI=" + item->parent()->text(0));
        sql_query.next();
        QString OIID = sql_query.value(0).toString();

        QSqlQuery sql_query2;
        sql_query2.exec("select * from oi_action where OIID= " + OIID + " and AttrNo = " + item->text(2));
        sql_query2.next();
        QString TypeCode = sql_query2.value(4).toString();

        QSqlQuery sql_query3;
        sql_query3.exec("select * from attr_detail where TypeCode= " + TypeCode);
        sql_query3.next();
        QString seq = sql_query3.value(3).toString();
        QString ItemVal = sql_query3.value(5).toString();
        QString DataType = sql_query3.value(6).toString();
        QString Idx = sql_query3.value(7).toString();
        QString Dot = sql_query3.value(8).toString();
        QString Unit = sql_query3.value(9).toString();
        QString Tremark = sql_query3.value(10).toString();
//        int EnEdit = sql_query3.value(11).toBool();


        auto newitem = new QTreeWidgetItem(ui->treeWidget_2);
        newitem->setText(0, item->parent()->text(0) + " " + item->parent()->text(1) + " " + item->text(1));
        newitem->setText(1, item->text(2));//属性
        newitem->setText(2, ItemVal);//数据值

        newitem->setText(3, DataType);//数据类型
        newitem->setText(4, Dot);//小数
        newitem->setText(5, Unit);//单位
        newitem->setText(6, Tremark);//备注
        newitem->setCheckState(0, Qt::Checked);


        PARAMETRIC new_m_itembox;
        new_m_itembox.left = item;
        new_m_itembox.right = newitem;
        Item_box.append(new_m_itembox);

    } else
    {
        delete need_del->right;
        delete need_del;
        Item_box.removeAt(i);
        flag = false;
    }
}

void _4_Parametric_variable::add_item(QTreeWidgetItem *item, QTreeWidgetItem *item2)
{
    if (item->parent())
    {
        if (item->checkState(0) == Qt::Checked)
            item->setCheckState(0, Qt::Unchecked);
        else
        {
            item->setCheckState(0, Qt::Checked);
        }
    }

}

void _4_Parametric_variable::EditFileFilter(QTreeWidgetItem *item, int column)
{
    if (column == 2 and item->text(3) != "structure")    //哪一列需要可编辑，这里是第二列
    {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    } else        //当双击的不是第二列的时候将flags中的可编辑属性去掉
    {
        item->setFlags(item->flags() & ~(Qt::ItemIsEditable));

    }


}

void _4_Parametric_variable::itemchanged(QTreeWidgetItem *item, QTreeWidgetItem *item2)
{
    if (item2 != NULL)
    {
        if (item2->text(3) == "array")
        {
            if (item2->text(2) != "0")
            {
                MEMBER_PRE new_stat;
                new_stat.num = item2->text(2);
                new_stat.pre_item = item2;
                        foreach(auto
                                        x, pre_stat)
                    {
                        if (x.num == new_stat.num and x.pre_item == new_stat.pre_item)
                        {
                            return;
                        }
                    }
                int times = item2->text(2).toInt();
                        foreach(auto
                                        x, ARRAY_box)
                    {
                        if (x.ItemName == item2->text(0))
                        {
                            if (item2->childCount() > 0)
                                item2->takeChildren();
                            for (int i = 0; i < times; i++)
                            {
                                QSqlQuery sql_query;
                                sql_query.exec("select * from attr_detail where PTypeCode=" + x.TypeCode);
                                while (sql_query.next())
                                {
                                    auto newitem = new QTreeWidgetItem(item2);
                                    newitem->setText(0, sql_query.value(4).toString());//name
                                    newitem->setText(2, sql_query.value(5).toString());//content
                                    newitem->setText(3, sql_query.value(6).toString());//DataType
                                    newitem->setText(4, sql_query.value(8).toString());
                                    newitem->setText(5, sql_query.value(9).toString());
                                    newitem->setText(6, sql_query.value(10).toString());
                                }
                                pre_stat.append(new_stat);

                            }
                        }
                    }
            } else
            {
                if (item2->childCount() > 0)
                    item2->takeChildren();
            }
        }
    }

}

void _4_Parametric_variable::check_cancel()
{
            foreach(auto x, Item_box)
        {
            x.right->setCheckState(0,Qt::Unchecked);
        }
}

void _4_Parametric_variable::list_clear()
{
            foreach(auto x, Item_box)
        {
            x.left->setCheckState(0,Qt::Unchecked);
        }
}


