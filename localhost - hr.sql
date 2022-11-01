set serveroutput on;

declare
v_empno     employees.employee_id%type;
v_name      employees.first_name%type;
v_sal       employees.salary%type; 
begin
    select employee_id, first_name, salary 
    into v_empno, v_name, v_sal 
    from employees
    where employee_id = '&사번';
    dbms_output.put_line(v_empno||' '||v_name||' '||v_sal);
    end;
    /

declare
v_empno     employees.employee_id%type;
v_name      employees.first_name%type;
v_sal       employees.salary%type;
v_hire_date employees.hire_date%type;
begin
    select employee_id, first_name, salary, hire_date
    into v_empno, v_name, v_sal, v_hire_date
    from employees
    where employee_id = '&사번';
    dbms_output.put_line(v_empno||' '||v_name||' '||v_sal||' '||v_hire_date);
    end;
    /
    
select employee_id, first_name, salary, hire_date
from employees
where employee_id = '&사번';

set verify off;

declare
v_no1   number := &no1;
v_no2   number := &no2;
v_sum   number;
begin
    v_sum := v_no1 + v_no2;
    dbms_output.put_line('첫번째 수:'||v_no1||', 두번째 수 : '||v_no2||', 합은 : '||v_sum||',입니다');
    end;
    /
    


-- PL/SQL 프로시저
create or replace procedure update_salary ( v_empno in number) /* IN agrument */
is
begin
    update employees
    set salary = salary * 1.1
    where employee_id = v_empno;
    commit;
    end update_salary;
    /
    
execute update_salary(110);
exec update_salary(100);

select * from employees where employee_id = 100;


-- PL/SQL 함수
create or replace function fc_update_salary(v_empno in number)
return number
is
    v_salary employees.salary%type;
begin
    select salary
    into v_salary
    from employees
    where employee_id = v_empno;
    
    update employees
    set salary = salary * 1.1
    where employee_id = v_empno;
    commit;
    
    select salary
    into v_salary
    from employees
    where employee_id = v_empno;
    
    return v_salary;
    end;
    /
    
var salary number;
execute :salary := fc_update_salary(111);
print salary;

select fc_update_salary(111) from dual;

/* RowType */
CREATE OR REPLACE procedure HR.rowtype_test
(p_empno in employees.employee_id%TYPE )
is
    v_emp employees%ROWTYPE;
begin 
    dbms_output.enable;

    select employee_id, first_name, hire_date
    into   v_emp.employee_id, v_emp.first_name,v_emp.hire_date
    FROM   employees
    where  employee_id = p_empno;

    DBMS_OUTPUT.PUT_LINE('EMP NO : '||v_emp.employee_id);
    DBMS_OUTPUT.PUT_LINE('EMP NAME : '||v_emp.first_name);
    DBMS_OUTPUT.PUT_LINE('EMP HIRE : '||v_emp.hire_date);

end;
/

execute rowtype_test(114);

/* PL/SQL 테이블 */ 
CREATE OR REPLACE PROCEDURE HR.TABLE_TEST
(v_deptno IN employees.DEPARTMENT_ID %TYPE)
IS
	-- 테이블의 선언
	TYPE empno_table IS TABLE OF employees.employee_id%TYPE INDEX BY BINARY_INTEGER;
	TYPE ename_table IS TABLE OF employees.first_name%TYPE INDEX BY BINARY_INTEGER;
	TYPE sal_table      IS TABLE OF employees.salary%TYPE INDEX BY BINARY_INTEGER;

	-- 테이블타입으로 변수 선언
	empno_tab empno_table ; 
	ename_tab ename_table ; 
	sal_tab   sal_table;
	i BINARY_INTEGER := 0;
BEGIN   
	DBMS_OUTPUT.ENABLE;

	-- FOR 루프 사용
                 -- 여기서 emp_list는 ( BINARY_INTEGER형 변수로) 1씩 증가
	FOR emp_list IN ( SELECT employee_id, first_name, salary
		        FROM employees 
		      WHERE department_id = v_deptno ) LOOP
	      i := i + 1;
	      -- 테이블 변수에 검색된 결과를 넣는다
		empno_tab(i) := emp_list.employee_id ;
		ename_tab(i) := emp_list.first_name ;
		sal_tab(i) := emp_list.salary ;
        
        
	END LOOP;
	
	-- 1부터 i까지 FOR 문을 실행
	FOR cnt IN 1..i LOOP
		-- TABLE변수에 넣은 값을 뿌려줌
		DBMS_OUTPUT.PUT_LINE( '사원번호 : ' || empno_tab(cnt) ); 
		DBMS_OUTPUT.PUT_LINE( '사원이름 : ' || ename_tab(cnt) ); 
		DBMS_OUTPUT.PUT_LINE( '사원급여 : ' || sal_tab(cnt));
	END LOOP; 
END TABLE_TEST;
/

execute table_test(100);

CREATE OR REPLACE PROCEDURE HR.RECORD_TEST(
p_eno IN employees.EMPLOYEE_ID%TYPE )
IS
	TYPE emp_record IS RECORD
	( v_eno NUMBER, 
	  v_nm  varchar2(30),
	  v_hdate DATE );
	
	emp_rec emp_record;
BEGIN
	DBMS_OUTPUT.ENABLE;
	
	SELECT e.EMPLOYEE_ID , 
	       e.FIRST_NAME||' '||e.LAST_NAME,
	       e.HIRE_DATE 
	INTO emp_rec.v_eno,
		 emp_rec.v_nm,
		 emp_rec.v_hdate
	FROM EMPLOYEES e 
	WHERE e.EMPLOYEE_ID = p_eno;

	DBMS_OUTPUT.PUT_LINE('사원 번호 :'|| emp_rec.v_eno);
	DBMS_OUTPUT.PUT_LINE('사원 이름 :'|| emp_rec.v_nm);
	DBMS_OUTPUT.PUT_LINE('입 사 일 :'|| emp_rec.v_hdate);

END RECORD_TEST;
/
execute record_test(100);

CREATE OR REPLACE PROCEDURE HR.TABLE_OF_RECORD
IS
    i BINARY_INTEGER := 0;
    -- PL/SQL Table of Record의 선언
    TYPE dept_table_type IS TABLE OF departments%ROWTYPE INDEX BY BINARY_INTEGER;
    dept_table dept_table_type;
BEGIN
    FOR dept_list IN (SELECT * FROM departments) LOOP
    i := i + 1;
    

    dept_table(i).department_id := dept_list.department_id;
    dept_table(i).department_name := dept_list.department_name;
    dept_table(i).location_id := dept_list.location_id;
    
    END LOOP;
    FOR cnt IN 1..i LOOP
        DBMS_OUTPUT.PUT_LINE(' DEPT NO : '||dept_table(cnt).department_id ||
                            ' DEPT NAME : '||dept_table(cnt).department_name ||
                            ' DEPT LOC : '||dept_table(cnt).location_id );
    END LOOP;
END;
/
execute TABLE_OF_RECORD;

create or replace procedure update_test
( v_empno in employees.employee_id%type,
  v_salary in employees.salary%type)
is
    v_emp employees%rowtype;
    -- 수정 데이터를 확인하기 위한 변수 선언
begin
    update employees
    set salary = v_salary
    where employee_id = v_empno;
    
    commit;
    
    dbms_output.put_line('Data Update Success');
    
    -- 수정된 데이터 확인하기 위한 검색
    select employee_id, last_name, salary
    into v_emp.employee_id, v_emp.last_name, v_emp.salary
    from employees
    where employee_id = v_empno;
    
    dbms_output.put_line(' **** Confirm Update Data ****');
    dbms_output.put_line(' EMP NO : '||v_emp.employee_id);
    dbms_output.put_line(' EMP NAME : '||v_emp.last_name);
    dbms_output.put_line(' EMP SALARY : '||v_emp.salary);
end;
/

execute update_test(100,10000);