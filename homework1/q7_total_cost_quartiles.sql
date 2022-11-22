select ifnull(CompanyName,'MISSING_NAME'),CustomerId,totalexpend
from
(select *,ntile(4) over(order by totalexpend) as ntile4
from
(select CompanyName,CustomerId,round(sum(OrderDetail.UnitPrice*OrderDetail.Quantity),2) as totalexpend
from 'Order' o natural left outer join (select Id as CustomerId,CompanyName from Customer) as Customer2,OrderDetail
where o.Id = OrderDetail.OrderId
group by o.CustomerId
order by totalexpend))
where ntile4 = 1







