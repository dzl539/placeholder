select R,F,L,B
from (select distinct RegionDescription as R,Employee.Id as Id,BirthDate as B,LastName as L,FirstName as F
from Employee,EmployeeTerritory,Territory,Region
where Employee.Id = EmployeeTerritory.EmployeeId and EmployeeTerritory.TerritoryId = Territory.Id and Territory.RegionId = Region.Id
order by Region.Id asc ,BirthDate desc) as M
where not exists (select * from (select distinct RegionDescription as R,Employee.Id as Id,BirthDate as B,LastName as L,FirstName as F
from Employee,EmployeeTerritory,Territory,Region
where Employee.Id = EmployeeTerritory.EmployeeId and EmployeeTerritory.TerritoryId = Territory.Id and Territory.RegionId = Region.Id
order by RegionDescription,BirthDate desc) as N where M.R = N.R and M.Id <>N.Id and N.B>M.B )
