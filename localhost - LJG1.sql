create table client(
    client_id           number(10),
    client_name         varchar2(100) not null,
    client_addr         varchar2(100),
    client_phoneNum     varchar2(100),
    client_type         varchar2(100),
    primary key(client_id)
);
commit;
/* 시퀀스 객체 생성 */
create SEQUENCE seq_client_id
INCREMENT by 1
start with 1000;
/* Client에 data 입력 */
insert into client
values (seq_client_id.nextval, '서울대학교병원', '서울특별시 종로구 혜화동','02-123-4567','대학병원');
/* data 입력 확인 */
select * from client;

create table item(
    item_id         number(10),
    item_name       varchar2(100) not null,
    item_cat        varchar2(100),
    item_color      varchar2(100),
    item_stock      number(20),
    item_price      number(20),
    primary key(item_id)
);
commit;
/* 시퀀스 객체 생성 */
create SEQUENCE seq_item_id
INCREMENT by 1
start with 880000;
/* Client에 data 입력 */
insert into item
values (seq_item_id.nextval, 'T1', '유니트체어','Blue',10,10000);
/* data 입력 확인 */
select * from item;

/* Order_app 테이블 생성 */
create table order_app(
    order_id            number(10),
    order_date          date,
    order_client_id     number(20),
    order_item_id       number(20),
    order_count         number(10),
    primary key(order_id),
    CONSTRAINT order_client_id_fk FOREIGN KEY (order_client_id)
    REFERENCES client(client_id),
    CONSTRAINT order_item_id_fk FOREIGN KEY (order_item_id)
    REFERENCES item(item_id)
);
commit;
/* 시퀀스 객체 생성 */
create SEQUENCE seq_order_id
INCREMENT by 1
start with 1;
/* Order에 data 입력 */
insert into order_app
values (seq_order_id.nextval, sysdate, 1000,880000,5);
/* data 입력 확인 */
select * from order_app;

select * from asdf;