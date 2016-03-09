# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0004_auto_20150616_0954'),
    ]

    operations = [
        migrations.AlterField(
            model_name='area',
            name='area_type',
            field=models.CharField(primary_key=True, db_column=b'area_type', default=b'12', choices=[(b'12', b'City'), (b'14', b'Area'), (b'17', b'Parking')], max_length=2, blank=True, verbose_name=b'area zoom'),
        ),
    ]
