import psycopg2

conn=psycopg2.connect(database="db_2020_01",user="db_user2020_271",password="db_user@123",host="116.205.157.173",port=8000)
cur=conn.cursor()

#认证过程
# x = ["bm", "aoe","campexe","csm","am"]
# cur.execute("SELECT id FROM SuperAdmin")  # 查询语句，以员工id和姓名为索引
# Supadmin = cur.fetchall()
# print(Supadmin[0])
#这一块也有点问题
Supadmin = ["bm", "aoe","campexe","csm","am"]
admin= ["bm", "aoe","campexe","csm","am"]
employee=["1", "2","3","4","5"]
Supadminpw = [1,2,3,4,5]
adminpw= [1,2,3,4,5]
emplpw = [1,2,3,4,5]

def Superadmin(flag):
    index=input("1.员工管理\t2.部门管理\t3.职位管理\t4.用户管理5.退出\n请输入前缀编号进入不同管理系统：\t")
    if index=="1":
        index1 = input("1.员工的增加\t2.员工的删除\t3.员工的修改\t4.员工的查询\t5.退出\n请输入前缀编号选择不同的操作：\t")
        if index1=="1":
            print("员工的增加")

            id=input("请输入员工的id：\t")
            name=input("请输入员工的姓名：\t")
            gender=input("请输入员工的性别：\t")
            birDate=input("请输入员工的出生日期：\t")
            EducationBackground=input("请输入员工的学历：\t")
            telnum=input("请输入员工的手机号码：\t")
            email=input("请输入员工的邮箱：\t")
            address=input("请输入员工的地址：\t")
            position=input("请输入员工的职位：\t")
            dept=input("请输入员工的部门：\t")
            HireDate=input("请输入员工的入职日期：\t")
            salary=input("请输入员工的薪水：\t")
            hobby=input("请输入员工的爱好：\t")
            cur.execute("INSERT INTO employee(id,name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby) VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",(id,name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby))
            conn.commit()
        if index1=="2":
            print("员工的删除")
            id1=input("请输入要删除的员工的id：\t")
            cur.execute("DELETE FROM employee where employee.id=%s",id1)#删除语句，以员工id为索引
            conn.commit()
        if index1=="3":
            print("员工的修改,根据员工的id进行修改，如需更改id，请删除后再添加")
            id = input("请输入员工的id：\t")
            name = input("请输入员工的姓名：\t")
            gender = input("请输入员工的性别：\t")
            birDate = input("请输入员工的出生日期：\t")
            EducationBackground = input("请输入员工的学历：\t")
            telnum = input("请输入员工的手机号码：\t")
            email = input("请输入员工的邮箱：\t")
            address = input("请输入员工的地址：\t")
            position = input("请输入员工的职位：\t")
            dept = input("请输入员工的部门：\t")
            HireDate = input("请输入员工的入职日期：\t")
            salary = input("请输入员工的薪水：\t")
            hobby = input("请输入员工的爱好：\t")
            cur.execute("UPDATE employee set name=%s,gender=%s,birDate=%s,EducationBackground=%s,telnum=%s,email=%s,address=%s,position=%s,dept=%s,HireDate=%s,salary=%s,hobby=%s where id=%s",(name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby,id))  # 修改语句，以员工id为索引
            conn.commit()
        if index1=="4":
            print("员工的查询,以员工id或姓名进行查询")

            id = input("请输入员工的id：\t")
            name = input("请输入员工的姓名：\t")
            cur.execute("SELECT * FROM employee where id=%s or name=%s",(id,name))  # 查询语句，以员工id和姓名为索引
            results = cur.fetchall()
            print(results)
            conn.commit()
        if index1=="5":
            flag=0

    if index == "2":
        index2 = input("1.部门的增加\t2.部门的删除\t3.部门的修改\t4.部门的查询\t5.退出\n请输入前缀编号选择不同的操作：\t")
        if index2 == "1":
            print("部门的增加")

            id = input("请输入部门的id：\t")
            name = input("请输入部门的名称：\t")
            #可修改
            creDate=input("请输入部门的创建时间：\t")
            ModiDate=input("请输入部门最后的修改时间：\t")
            cur.execute(
                "INSERT INTO deptment(id,name,creDate,ModiDate) VALUES(%s,%s,%s,%s)",
                (id, name, creDate,ModiDate))
            conn.commit()
        if index2 == "2":
            print("部门的删除(用id删除或名称删除，只填写一个，另一个写无即可)")
            id1 = input("请输入要删除的部门的id：\t")
            name= input("请输入要删除的部门的名称：\t")
            cur.execute("DELETE FROM deptment where id=%s or name=%s", (id1,name))  # 删除语句，以员工id为索引
            conn.commit()
        if index2 == "3":
            print("部门的修改,根据部门的id进行修改，如需更改id，请删除后再添加")
            id = input("请输入部门的id：\t")
            name = input("请输入部门的姓名：\t")
            # 可修改
            creDate = input("请输入部门的创建时间：\t")
            ModiDate = input("请输入部门最后的修改时间：\t")
            cur.execute(
                "UPDATE deptment set name=%s,creDate=%s,ModiDate=%s where id=%s",
                (name, creDate,ModiDate, id))  # 修改语句，以员工id为索引
            conn.commit()
        if index2 == "4":
            print("部门的查询,以部门id或姓名进行查询")

            id = input("请输入部门的id：\t")
            name = input("请输入部门的姓名：\t")
            cur.execute("SELECT * FROM deptment where id=%s or name=%s", (id, name))  # 查询语句，以员工id和姓名为索引
            results = cur.fetchall()
            print(results)
            conn.commit()
        if index2 == "5":
            flag = 0

    if index=="3":
        index3 = input("1.职位的增加\t2.职位的删除\t3.职位的修改\t4.职位的查询\t5.退出\n请输入前缀编号选择不同的操作：\t")
        if index3 == "1":
            print("职位的增加")

            id = input("请输入职位的id：\t")
            name = input("请输入职位的名称：\t")
            PAdmin=input("请输入管理人：\t")
            PDept=input("请输入所属部门：\t")
            # 可修改
            creDate = input("请输入职位的创建时间：\t")
            ModiDate = input("请输入职位最后的修改时间：\t")
            cur.execute(
                "INSERT INTO position(id,name,PAdmin,PDept,creDate,ModiDate) VALUES(%s,%s,%s,%s,%s,%s)",
                (id, name, PAdmin,PDept,creDate, ModiDate))
            conn.commit()
        if index3 == "2":
            print("职位的删除(用id删除或名称删除，只填写一个，另一个写无即可)")
            id1 = input("请输入要删除的职位的id：\t")
            name = input("请输入要删除的职位的名称：\t")
            cur.execute("DELETE FROM position where id=%s or name=%s", (id1, name))  # 删除语句，以员工id为索引
            conn.commit()
        if index3 == "3":
            print("职位的修改,根据职位的id进行修改，如需更改id，请删除后再添加")
            id = input("请输入职位的id：\t")
            name = input("请输入职位的姓名：\t")
            PAdmin=input("请输入管理人：\t")
            PDept=input("请输入所属部门：\t")
            # 可修改
            creDate = input("请输入职位的创建时间：\t")
            ModiDate = input("请输入职位最后的修改时间：\t")
            cur.execute(
                "UPDATE position set name=%s,PAdmin=%s,PDept=%s,creDate=%s,ModiDate=%s where id=%s",
                (name, PAdmin,PDept,creDate, ModiDate, id))  # 修改语句，以员工id为索引
            conn.commit()
        if index3 == "4":
            print("职位的查询,以职位id或姓名进行查询")

            id = input("请输入职位的id：\t")
            name = input("请输入职位的姓名：\t")
            cur.execute("SELECT * FROM position where id=%s or name=%s", (id, name))  # 查询语句，以员工id和姓名为索引
            results = cur.fetchall()
            print(results)
            conn.commit()
        if index3 == "5":
            flag = 0

    if index=="4":
        index4 = input("1.角色的增加\t2.角色的删除\t3.角色的修改\t4.角色的查询\t5.退出\n请输入前缀编号选择不同的操作：\t")
        if index4 == "1":
            print("角色的增加")
            index5=input("对具体哪种角色进行修改：1.supadmin\t2.admin\t3.employee\n请输入前缀编号选择不同的操作：\t")
            if index5=="1":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                cur.execute(
                    "INSERT INTO SuperAdmin(id,password) VALUES(%s,%s)",
                    (id, password))
                conn.commit()
            if index5=="2":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                cur.execute(
                    "INSERT INTO Admin(id,password) VALUES(%s,%s)",
                    (id, password))
                conn.commit()
            if index5=="3":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                cur.execute(
                    "INSERT INTO EmployeeR(id,password) VALUES(%s,%s)",
                    (id, password))
                conn.commit()

        if index4 == "2":
            print("角色的删除")
            index5=input("对具体哪种角色进行修改：1.supadmin\t2.admin\t3.employee\n请输入前缀编号选择不同的操作：\t")
            if index5=="1":
                id = input("请输入角色的id：\t")
                # password = input("请输入角色的密码：\t")
                cur.execute("DELETE FROM SuperAdmin where id=%s", id)
                conn.commit()
            if index5=="2":
                id = input("请输入角色的id：\t")
                # password = input("请输入角色的密码：\t")
                cur.execute(
                    "DELETE FROM Admin where id=%s",
                    id)
                conn.commit()
            if index5=="3":
                id = input("请输入角色的id：\t")
                # password = input("请输入角色的密码：\t")
                cur.execute(
                    "DELETE FROM EmployeeR where id=%s", id)
                conn.commit()

        if index4 == "3":
            print("角色密码的修改")
            index5 = input("对具体哪种角色进行修改：1.supadmin\t2.admin\t3.employee\n请输入前缀编号选择不同的操作：\t")
            if index5 == "1":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                # "UPDATE position set name=%s,gender=%s,PAdmin=%s,PDept=%s,creDate=%s,ModiDate=%s where id=%s"
                cur.execute("UPDATE SuperAdmin set password=%s where id=%s", (password,id))
                conn.commit()
            if index5 == "2":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                cur.execute(
                    "UPDATE Admin set password=%s where id=%s", (password,id))
                conn.commit()
            if index5 == "3":
                id = input("请输入角色的id：\t")
                password = input("请输入角色的密码：\t")
                cur.execute(
                    "UPDATE EmployeeR set password=%s where id=%s", (password,id))
                conn.commit()

        if index4 == "4":
            print("角色的查询")
            index5 = input("对具体哪种角色进行查询：1.supadmin\t2.admin\t3.employee\n请输入前缀编号选择不同的操作：\t")
            if index5 == "1":
                cur.execute("SELECT * from SuperAdmin")
                conn.commit()
                results = cur.fetchall()

                print(results)
            if index5 == "2":
                cur.execute(
                    "SELECT * from Admin")
                results = cur.fetchall()

                print(results)
                conn.commit()
            if index5 == "3":
                cur.execute(
                    "SELECT * from EmployeeR")
                results = cur.fetchall()

                print(results)
                conn.commit()

        if index4 == "5":
            flag = 0

    if index == "5":
        flag=0
        #退出这一块还是有点问题

    return flag

def Admin():
    index="1"
    if index=="1":
        index1 = input("1.员工的增加\t2.员工的删除\t3.员工的修改\t4.员工的查询\t5.退出\n请输入前缀编号选择不同的操作：\t")
        if index1=="1":
            print("员工的增加")

            id=input("请输入员工的id：\t")
            name=input("请输入员工的姓名：\t")
            gender=input("请输入员工的性别：\t")
            birDate=input("请输入员工的出生日期：\t")
            EducationBackground=input("请输入员工的学历：\t")
            telnum=input("请输入员工的手机号码：\t")
            email=input("请输入员工的邮箱：\t")
            address=input("请输入员工的地址：\t")
            position=input("请输入员工的职位：\t")
            dept=input("请输入员工的部门：\t")
            HireDate=input("请输入员工的入职日期：\t")
            salary=input("请输入员工的薪水：\t")
            hobby=input("请输入员工的爱好：\t")
            cur.execute("INSERT INTO employee(id,name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby) VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",(id,name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby))
            conn.commit()
        if index1=="2":
            print("员工的删除")
            id1=input("请输入要删除的员工的id：\t")
            cur.execute("DELETE FROM employee where employee.id=%s",id1)#删除语句，以员工id为索引
            conn.commit()
        if index1=="3":
            print("员工的修改,根据员工的id进行修改，如需更改id，请删除后再添加")
            id = input("请输入员工的id：\t")
            name = input("请输入员工的姓名：\t")
            gender = input("请输入员工的性别：\t")
            birDate = input("请输入员工的出生日期：\t")
            EducationBackground = input("请输入员工的学历：\t")
            telnum = input("请输入员工的手机号码：\t")
            email = input("请输入员工的邮箱：\t")
            address = input("请输入员工的地址：\t")
            position = input("请输入员工的职位：\t")
            dept = input("请输入员工的部门：\t")
            HireDate = input("请输入员工的入职日期：\t")
            salary = input("请输入员工的薪水：\t")
            hobby = input("请输入员工的爱好：\t")
            cur.execute("UPDATE employee set name=%s,gender=%s,birDate=%s,EducationBackground=%s,telnum=%s,email=%s,address=%s,position=%s,dept=%s,HireDate=%s,salary=%s,hobby=%s where id=%s",(name,gender,birDate,EducationBackground,telnum,email,address,position,dept,HireDate,salary,hobby,id))  # 修改语句，以员工id为索引
            conn.commit()
        if index1=="4":
            print("员工的查询,以员工id或姓名进行查询")

            id = input("请输入员工的id：\t")
            name = input("请输入员工的姓名：\t")
            cur.execute("SELECT * FROM employee where id=%s or name=%s",(id,name))  # 查询语句，以员工id和姓名为索引
            results = cur.fetchall()
            print(results)
            conn.commit()
        if index1=="5":
            flag=0

    return 0

def EMployee(id):
    # print(id)
    cur.execute("SELECT * FROM employee where id=%s", (id))  # 查询语句，以员工id和姓名为索引
    results = cur.fetchall()
    print(results)
    conn.commit()
    flag=0

    return 0


try:
    while 1:
        print("提示：只能输入以下角色：supadmin，admin，employee")
        inprole = input("Role:\t")
        if inprole=="supadmin":

            inpsupadmin = input("SupAdmin:\t")
            if inpsupadmin in Supadmin:
                index = Supadmin.index(inpsupadmin)
                inppass = int(input("Password:\t"))
                if inppass == Supadminpw[index]:
                    print("Successfully Logged in....!")
                    flag=1
                    while(flag):
                        Superadmin(flag)
                        # print(flag)
                    # break
                else:
                    print("Incorrect Password...!")
                    continue
            else:
                print("No username exists.")
            continue


        if inprole=="admin":
            inpadmin = input("Admin:\t")
            if inpadmin in admin:
                index = admin.index(inpadmin)
                inppass = int(input("Password:\t"))
                if inppass == adminpw[index]:
                    print("Successfully Logged in....!")
                    while (1):
                        Admin()
                    # break
                else:
                    print("Incorrect Password...!")
                    continue
            else:
                print("No username exists.")
            continue

        if inprole == "employee":
            inpemp = input("employee:\t")
            if inpemp in employee:
                index = employee.index(inpemp)
                inppass = int(input("Password:\t"))
                if inppass == emplpw[index]:
                    print("Successfully Logged in....!")
                    EMployee(inpemp)
                    # break
                else:
                    print("Incorrect Password...!")
                    continue
            else:
                print("No username exists.")
            continue
except:
    print("Error logging in")


conn.close()