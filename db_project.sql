------------------------------Client -----------------
/* 시퀀스 객체 생성 */   
create SEQUENCE seq_client_id
INCREMENT by 1
start with 1004; 
select * from client;
/* 데이터 임포트한 txt 파일에 PK,FK 부여 */
alter table client add primary key (client_id);
commit;
---------------------------- Item -----------------
/* 시퀀스 객체 생성 */
create SEQUENCE seq_item_id
INCREMENT by 1
start with 880003;
/* 데이터 임포트한 txt 파일에 PK,FK 부여 */
alter table item add primary key (item_id);
commit;
select * from item;

---------------------------------------- Order ------------------
/* 시퀀스 객체 생성 */ 
create SEQUENCE seq_order_id
INCREMENT by 1
start with 4; 
/* 데이터 임포트한 txt 파일에 PK,FK 부여 */
alter table order_app add primary key (order_id);
alter table order_app add CONSTRAINT o_client_id_fk foreign key (order_client_id) REFERENCES client(client_id);
alter table order_app add CONSTRAINT o_item_id_fk foreign key (order_item_id) REFERENCES item(item_id);
commit;
select * from order_app;
/
---------------------- Client Add Procedure --------------------
create or replace procedure client_add
( --v_client_id in client.client_id%type,
  v_client_name in client.client_name%type,
  v_client_addr in client.client_addr%type,
  v_client_phonenum in client.client_phonenum%type,
  v_client_type in client.client_type%type )
is
begin
    insert into client
    (client_id, client_name, client_addr, client_phonenum, client_type)
    values (seq_client_id.nextval, v_client_name, v_client_addr, v_client_phonenum, v_client_type);
    commit;
    dbms_output.put_line('client_name : '||v_client_name);
    dbms_output.put_line('client_addr : '||v_client_addr);
    dbms_output.put_line('client_phonenum : '||v_client_phonenum);
    dbms_output.put_line('client_type : '||v_client_type);
end;
/
--------------------- Client Update Procedure ----------------
create or replace procedure client_update
( v_client_id in client.client_id%type,
  v_client_name in client.client_name%type,
  v_client_addr in client.client_addr%type,
  v_client_phonenum in client.client_phonenum%type,
  v_client_type in client.client_type%type )
is
  v_client client%rowtype;
begin
    update client
    set --client_id = v_client_id,
        client_name = v_client_name,
        client_addr = v_client_addr,
        client_phonenum = v_client_phonenum,
        client_type = v_client_type
    where client_id = v_client_id;
    commit;
end;
/

--------------------- Client Remove Procedure ------------------
create or replace procedure client_remove
(   v_client_id in client.client_id%type)
--    v_client_name in client.client_name%type,
--    v_client_addr in client.client_addr%type,
--    v_client_phonenum in client.client_phonenum%type,
--    v_client_type in client.client_type%type)
is
begin
    delete from client where client_id = v_client_id;
    commit;
end;
/ 

---------------------- Item Add Procedure --------------------
create or replace procedure item_add
( v_item_id in item.item_id%type,
  v_item_name in item.item_name%type,
  v_item_cat in item.item_cat%type,
  v_item_color in item.item_color%type,
  v_item_stock in item.item_stock%type,
  v_item_price in item.item_price%type)
is
begin
    insert into item
    (item_id, item_name, item_cat, item_color, item_stock, item_price)
    values (seq_item_id.nextval, v_item_name, v_item_cat, v_item_color, v_item_stock,v_item_price);
    commit; 
end;
/

--------------------- Item Update Procedure ----------------
create or replace procedure item_update
( v_item_id in item.item_id%type,
  v_item_name in item.item_name%type,
  v_item_cat in item.item_cat%type,
  v_item_color in item.item_color%type,
  v_item_stock in item.item_stock%type,
  v_item_price in item.item_price%type)
is 
begin
    update item
    set
        item_name = v_item_name,
        item_cat = v_item_cat,
        item_color = v_item_color,
        item_stock = v_item_stock,
        item_price = v_item_price
    where item_id = v_item_id;
    commit;
end;
/

--------------------- Item Remove Procedure ------------------
create or replace procedure item_remove
(   v_item_id in item.item_id%type) 
is
begin
    delete from item where item_id = v_item_id;
    commit;
end;
/ 