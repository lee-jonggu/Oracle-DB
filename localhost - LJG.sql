create table book(
    book_id     number(5),
    title       varchar2(50),
    author      varchar2(10),
    pub_date    date
);

select * from book;

alter table book add (pubs varchar2(50));
commit;
select * from book;

alter table book modify (title varchar2(100));
commit;

alter table book drop (author);
commit;
select * from book;

rename book to article;
select * from book;
select * from article;

drop table article;
select * from article;
drop table book;
commit;
select * from author;
select * from book;
create table book(
    author_id       number(10),
    author_name     varchar2(100),
    author_desc     varchar2(500),
    primary key(author_id)
);
commit;
rename book to author;

select * from author;
alter table author modify (author_name varchar2(100) NOT NULL);
commit;

create table book(
    book_id     number(10),
    title       varchar2(100) not null,
    pubs        varchar2(100),
    pub_date    date,
    author_id   number(10),
    primary key(book_id),
    CONSTRAINT c_book_fk FOREIGN KEY (author_id)
    REFERENCES author(author_id)
);
commit;

insert into author
values (1, '�ڰ渮', '���� �۰�');

insert into author( author_id, author_name)
values (2, '�̹���');

select * from author;

update author
set author_name = '���84',
    author_desc = '�����۰�'
where author_id = 1;
commit;

update author
set author_name = '��Ǯ',
    author_desc = '�α��۰�';

select * from author where author_id = 1;    
delete from author
where author_id = 1;

/* ������ ��ü ���� */
create SEQUENCE seq_author_id
INCREMENT by 1
start with 1;

select * from author;
select seq_author_id.nextval from dual;
delete from author where author_id = 2;

drop SEQUENCE seq_author_id;

insert into author
values (seq_author_id.nextval, '�ڰ渮' ,'���� �۰�');

insert into author
values (seq_author_id.nextval, '�̹���', '�ﱹ�� �۰�');

select * from author;

update author
set author_desc = '��� ����'
where author_id = 1;
commit;

update author
set author_desc = '��󳲵� �뿵'
where author_id = 2;
commit;

select * from author;

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '���ù�', '17�� ��ȸ�ǿ�');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '���84', '��ȵ����� �� 84���');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '��Ǯ', '�¶��� ��ȭ�� 1����');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '�迵��', '�˾�����');

select * from author;

/* ������ ��ü ���� */
drop sequence seq_book_id;
create SEQUENCE seq_book_id
INCREMENT by 1
start with 1;
select * from book;
insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '�츮���� �ϱ׷��� ����','�ٸ�', '1998-02-22' ,'1');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '�ﱹ��','������', '2002-03-01' ,'1');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '����','���δϿ��Ͻ�', '2012-08-15' ,'2');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '���ù��� �۾��� Ư��','������ ��', '2015-04-01' ,'3');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '�мǿ�','�߾ӺϽ�(books)', '2012-02-22' ,'4');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '������ȭ','�������', '2011-08-03' ,'5');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '�����λ��','���е���', '2017-05-04' ,'6');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '26��','�������', '2012-02-04' ,'5');
commit;
select * from book;
select * from author;

update author
set author_name = '�ڰ渮'
where author_id = 2;

select b.book_id, b.title, b.pubs, to_char(b.pub_date,'yyyy-mm-dd'), a.author_id, a.author_name, a.author_desc 
from author a, book b
where a.author_id = b.author_id
order by b.book_id;

select *
from author a, book b
where a.author_id = b.author_id
order by b.book_id;

select * from author;

delete 
from author 
where author_name = '���84';

delete 
from author 
where author_id =  4;

select * from book;

delete
from book
where book_id = 5;

rollback;

select count(*) from seoul;