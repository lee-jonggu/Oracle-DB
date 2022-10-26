/* 2022-10-26 SQL */

select employee_id, first_name, salary
from employees
where salary > (select salary from employees where first_name = 'Den');

select first_name, salary, employee_id
from employees
where salary = (select min(salary) from employees);

select first_name, salary, employee_id
from employees
where salary < (select avg(nvl((salary),0)) from employees);


select department_id, employee_id, first_name, salary
from employees
where (department_id, salary) in (select department_id, max(salary)
                                from employees
                                group by department_id);
                                
select e.department_id, e.employee_id, e.first_name, e.salary
from employees e, (select department_id, max(salary) as salary
                                from employees
                                group by department_id) s
where e.department_id = s.department_id
and e.salary = s.salary;


-- 1
select count(*)
from employees
where salary < (select avg(salary) from employees);

-- 2
select department_id, employee_id, first_name, salary
from employees
where (department_id, salary) in (select department_id, max(salary)
                                from employees
                                group by department_id);
                                
-- 3
select j.job_title, e.salary
from jobs j, (select job_id, sum(salary) as salary
                            from employees
                            group by job_id) e
where e.job_id = j.job_id
order by 2 desc;

-- 4
select e.employee_id, e.last_name, e.salary as "급여" , e2.salary as "부서 평균 급여", e.department_id as "부서"
from employees e, (select department_id, round(avg(salary)) as salary
                    from employees
                    group by department_id) e2
where e.department_id = e2.department_id
and e.salary > e2.salary;

select rownum, first_name, salary
from (select first_name, salary 
        from employees 
        order by 2 desc);

select first_name, salary
from (select rownum, first_name, salary
        from (select first_name, salary 
                from employees 
                order by 2 desc))
where rownum < 5;



-- hr 실습 예제 5

-- 1
select e.first_name, e.last_name, e.salary, d.department_name, e.hire_date
from employees e, departments d
where e.hire_date = (select max(hire_date) from employees)
and e.department_id = d.department_id;

-- 2
select e.employee_id, e.first_name, e.last_name, j.job_title, s.sal
from jobs j, employees e, (select department_id, avg(salary) as sal
                            from employees
                            group by department_id) s
where e.department_id = s.department_id
and e.job_id = j.job_id;

