using Bogus;
using prakt9.Classes;

int[] groups_id = new int[5] { 1, 2, 3, 4, 5 };
Random rnd = new Random();

Faker<Student> faker = new Faker<Student>("ru")
    .RuleFor(s => s.Name, f => f.Person.FirstName)
    .RuleFor(s => s.Surname, f => f.Person.LastName)
    .RuleFor(s => s.Birthday, f => f.Date.Between(new DateTime(2000, 01, 01),
    new DateTime(2007, 01, 01)))
    .RuleFor(s => s.PhoneNumber, f => f.Phone.PhoneNumber("+7(###)-###-####"))
    .RuleFor(c => c.Passport, f => f.Phone.PhoneNumber("#### #####"))
    .RuleFor(c => c.GroupId, f => f.Random.ArrayElement(groups_id))
    .RuleFor(c => c.Email, f => f.Internet.Email())
    .RuleFor(c => c.IsBudget, f => f.Random.Number(0,1));

List<Student> students = faker.Generate(10);

foreach (Student student in students)
{
    Console.WriteLine($"{student.Name} {student.Surname} " +
        $"{student.Birthday.ToShortDateString()} " +
        $"{student.PhoneNumber} Passport:{student.Passport} ");
}