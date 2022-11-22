select C.CategoryName,count(P.ProductName),round(avg(P.UnitPrice),2),min(P.UnitPrice),max(P.UnitPrice),sum(P.UnitsOnOrder)
from Product as P,Category as C
where P.CategoryId = C.Id and C.Id in(select Id
from(select C.Id as Id,count(P.ProductName) as ProductsNumber
from Product as P,Category as C
where P.CategoryId = C.Id 
group by C.Id)
where ProductsNumber > 10)
group by C.Id
order by C.Id

