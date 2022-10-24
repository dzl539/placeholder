
select Product.ProductName,Customer.CompanyName, Customer.ContactName

from 'Customer','Order' as 'O','OrderDetail','Product'

where Customer.Id = O.CustomerId and O.Id = OrderDetail.OrderId and OrderDetail.ProductId = Product.Id and Product.Discontinued = 1 and exists(select * 
from (select Product.ProductName as P ,min(o2.OrderDate) as Nummin 
from 'Order' as o2,'OrderDetail','Product'
where o2.Id = OrderDetail.OrderId and OrderDetail.ProductId = Product.Id and Product.Discontinued = 1 group by Product.ProductName) as MCS
where  O.OrderDate = MCS.Nummin and Product.ProductName = MCS.P)

group by Product.ProductName
order by Product.ProductName asc





