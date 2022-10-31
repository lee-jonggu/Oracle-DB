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
values (1, '박경리', '토지 작가');

insert into author( author_id, author_name)
values (2, '이문열');

select * from author;

update author
set author_name = '기안84',
    author_desc = '웹툰작가'
where author_id = 1;
commit;

update author
set author_name = '강풀',
    author_desc = '인기작가';

select * from author where author_id = 1;    
delete from author
where author_id = 1;

/* 시퀀스 객체 생성 */
create SEQUENCE seq_author_id
INCREMENT by 1
start with 1;

select * from author;
select seq_author_id.nextval from dual;
delete from author where author_id = 2;

drop SEQUENCE seq_author_id;

insert into author
values (seq_author_id.nextval, '박경리' ,'토지 작가');

insert into author
values (seq_author_id.nextval, '이문열', '삼국지 작가');

select * from author;

update author
set author_desc = '경북 영양'
where author_id = 1;
commit;

update author
set author_desc = '경상남도 통영'
where author_id = 2;
commit;

select * from author;

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '유시민', '17대 국회의원');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '기안84', '기안동에서 산 84년생');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '강풀', '온라인 만화가 1세대');

insert into author( author_id, author_name, author_desc)
values (seq_author_id.nextval, '김영하', '알쓸신잡');

select * from author;

/* 시퀀스 객체 생성 */
drop sequence seq_book_id;
create SEQUENCE seq_book_id
INCREMENT by 1
start with 1;
select * from book;
insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '우리들의 일그러진 영웅','다림', '1998-02-22' ,'1');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '삼국지','민음사', '2002-03-01' ,'1');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '토지','마로니에북스', '2012-08-15' ,'2');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '유시민의 글쓰기 특강','생각의 길', '2015-04-01' ,'3');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '패션왕','중앙북스(books)', '2012-02-22' ,'4');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '순정만화','재미주의', '2011-08-03' ,'5');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '오직두사람','문학동네', '2017-05-04' ,'6');

insert into book ( book_id, title, pubs, pub_date, author_id)
values (seq_book_id.nextval, '26년','재미주의', '2012-02-04' ,'5');
commit;
select * from book;
select * from author;

update author
set author_name = '박경리'
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
where author_name = '기안84';

delete 
from author 
where author_id =  4;

select * from book;

delete
from book
where book_id = 5;

rollback;

select count(*) from seoul;