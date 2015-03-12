DELETE FROM sg_sctphost;
INSERT INTO sg_sctphost VALUES ('1', 'Y', '0', '172.25.126.10', NULL);
INSERT INTO sg_sctphost VALUES ('3', 'N', '0', '172.25.126.16', NULL);

DELETE FROM sg_asp;
INSERT INTO sg_asp VALUES ('1', '2905', '1', 'O', '1', 'N');

DELETE FROM sg_sgp;
INSERT INTO sg_sgp VALUES ('1', '2905', '3', '1');

DELETE FROM sg_asso;
INSERT INTO sg_asso VALUES ('1', 'M3UA', '2905', '1', '2905', '3', '1', '1', '3000', '1000', '60000', '8', '4', '60', '10', '5', '8', '30', '3000', '4', '4', 'N', 'CLIENT', '0', '1500', 'F', '100', NULL, NULL);

DELETE FROM sg_sg;
INSERT INTO sg_sg VALUES ('1', 'THIS_IS_SG1', 'O', '1', '0', '0', '0', '0', '2', 'ITU');
DELETE FROM sg_as;
INSERT INTO sg_as VALUES ('1', 'L', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'RKD', 'TPC', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'NONE');

DELETE FROM sg_pc;
INSERT INTO sg_pc VALUES ('0', '0', '0', '2', '1', '0', '0', 'ITU', 'NONE', 'O');

