

select CompanyName,round((latenumber*1.0)/(allnumber*1.0)*100,2) 
from
(select Shipper.CompanyName,count(o.Id) as latenumber
from 'Order' as o,'Shipper'
where (o.ShipVia = Shipper.Id) and (o.ShippedDate > o.RequiredDate)
group by Shipper.CompanyName) natural join
(select Shipper.CompanyName,count(o.Id) as allnumber
from 'Order' as o,'Shipper'
where (o.ShipVia = Shipper.Id)
group by Shipper.CompanyName)


