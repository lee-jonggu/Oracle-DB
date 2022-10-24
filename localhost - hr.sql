select * from EMPLOYEES;

select * from departments d;

select employee_id, first_name, last_name from employees;

select first_name, phone_number, hire_date, salary from employees;

select first_name, last_name, salary, phone_number,email, hire_date from employees e;

select employee_id as empNo, first_name "E-name", salary "연 봉" from employees;

select first_name "이름", 
        last_name "성", 
        salary "급여",
        phone_number "전화번호",
        email "이메일",
        hire_date "입사일" 
from employees e;

select first_name || ' ' || last_name as "name" from employees;

select first_name || ' hire date is ' || hire_date from employees;

select first_name, salary, salary*12, (salary+300)*12 from employees;

--select job_id*12 from employees; -- job_id 는 int가 아니다

select first_name || '-' || last_name as "name",
    salary as "급여",
    salary*12 as "연봉",
    salary*12 + 5000 as "연봉 2",
    phone_number as "전화번호"
    from employees; 
    
select first_name from employees where department_id = 10;

select employee_id, first_name, salary, salary*12 from employees where salary >= 15000;

select first_name, hire_date from employees where hire_date >= '07/01/01';
select first_name, hire_date from employees where hire_date > '06/12/31';
select TO_CHAR(hire_date, 'yyyy-mm-dd') from employees;

select first_name, salary from employees where first_name = 'Lex';

select first_name, salary from employees where salary between 14000 and 17000;

select employee_id, first_name, salary*12, job_id from employees where salary <= 14000 or salary >= 17000;

select first_name, hire_date from employees where hire_date >= '04/01/01' and hire_date <= '05/12/31';

select first_name, last_name, salary from employees where first_name in ('Neena', 'Lex' ,'John');
select first_name, last_name, salary from employees where salary in (2100,3100,4100,5100);

select first_name, last_name, salary from employees where first_name like 'L%';
select first_name, last_name, salary from employees where first_name like '%am%';
select first_name, last_name, salary from employees where first_name like '_a%';
select first_name, last_name, salary from employees where first_name like '___a%';
select first_name, last_name, salary from employees where first_name like '__a_';

select first_name from employees where manager_id is null and commission_pct is null;

select department_id, salary, first_name
from employees 
order by 1;

select first_name, salary 
from employees
where salary >= 5000
order by 2 desc;

select department_id, salary, first_name
from employees
order by 1 asc, 2 desc;

select avg(salary),sum(salary) from employees;

select CHR(65) from dual;
select ASCII('A') from dual;

select email, initcap(email), department_id
from employees
where department_id = 100;

select first_name , lower(first_name), upper(first_name)
from employees
where department_id = 100;

select first_name, substr(first_name,1,3), substr(first_name,-3,2)
from employees
where department_id = 100;

select first_name, lpad(first_name,10,'*'), rpad(first_name,10,'*') from employees;

select first_name, replace(first_name, 'a', '*') from employees where first_name like '%a%a%';

select first_name, replace(first_name, 'a', '*'), replace(first_name, substr(first_name,2,3), '****')
from employees 
where department_id = 100;

select round(123.456, -1) from dual; 

select TO_CHAR(sysdate, 'yyyy-mm-dd hh24:mm:ss') from dual;
select sysdate from employees;

select first_name, hire_date, months_between(sysdate, hire_date)
from employees
where department_id = 110;

select first_name, to_char(salary*12, '$999,999.99') as "SAL"
from employees
where department_id = 110;

select to_char(sysdate, 'mon') from dual;

select TO_CHAR(sysdate, 'yyyy"년"-mm"월"-dd"일" hh24:mm:ss') as "Date" from dual;

select commission_pct, nvl(commission_pct,0) from employees;

select commission_pct, nvl2(commission_pct,commission_pct,0) from employees;

-- hr 계정 SQL 실습문제_기본
-- 1
select first_name as "이름", last_name, salary, phone_number, hire_date
from employees
order by hire_date;

-- 2 
select job_id, salary from employees
order by 1 desc, 2 desc;

-- 3
select first_name, manager_id, commission_pct, salary
from employees
where manager_id is not null 
and commission_pct is null;

-- 4
select job_title, max_salary
from jobs
where max_salary >= 10000
order by max_salary desc;

-- 5
select first_name, salary, nvl2(commission_pct, commission_pct,0) 
from employees
where salary < 14000 and salary >= 10000
order by salary desc;

-- 6
select first_name, salary, to_char(hire_date,'yyyy-mm'), department_id
from employees
where department_id in (10,90,100);
--where department_id = 10
--or department_id = 90
--or department_id = 100;

-- 7
select first_name, salary
from employees
--where first_name like 'S%' 
--or first_name like '%s%';
where upper(first_name) like '%S%';

-- 8
select d.department_name, e.department_id
from employees e, departments d
where e.department_id = d.department_id(+)
order by length(d.department_name) desc;

select department_name
from departments
order by length(department_name) desc;

-- 9
select upper(country_name)
from countries
order by 1 asc; 
select DISTINCT(upper(c.country_name))
from countries c, locations l, departments d
where c.country_id = l.country_id
and l.location_id = d.location_id; 

-- 10
select first_name, salary, replace(phone_number,'.','-'), hire_date
from employees
where hire_date <= '03/12/31';

select * from employees;